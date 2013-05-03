
#include "stdafx.h"
#include "BasicC2SHandler.h"
#include "../Utility/ServerUserAccess.h"
#include "../Algorithm/GroupTraverse.h"
#include "Network/ErrReport/ErrorCheck.h"

#include "NetProtocol/Src/basic_ProtocolListener.cpp"
#include "NetProtocol/Src/basic_Protocol.cpp"

using namespace network;
using namespace network::error;

CBasicC2SHandler::CBasicC2SHandler( CServerBasic &svr ) :
	m_Server(svr)
{
	svr.RegisterProtocol(&m_BasicProtocol);
}

CBasicC2SHandler::~CBasicC2SHandler()
{
}


/**
 @brief ReqLogIn
 */
bool CBasicC2SHandler::ReqLogIn(IProtocolDispatcher &dispatcher, netid senderId, const std::string &id, 
	const std::string &passwd, const certify_key &c_key)
{
	CSession *pClient = m_Server.GetSession(id);
	if (pClient)
	{
		if (pClient->GetState() == SESSIONSTATE_LOGIN)
		{
			clog::Error( clog::ERROR_PROBLEM, "ReqLogin Error!! client already exist senderId=%d, id=%s",
				senderId, id.c_str());
			m_BasicProtocol.AckLogIn(senderId, SEND_T, error::ERR_ALREADY_EXIST_USER, id, 0);
			return false;
		}
	}

	pClient = m_Server.GetSession(senderId);
	if (!pClient)
	{
		clog::Error( clog::ERROR_PROBLEM, "ReqLogin Error!! client not found senderId=%d, id=%s",
			senderId, id.c_str());
		m_BasicProtocol.AckLogIn(senderId, SEND_T, error::ERR_NOT_FOUND_USER, id, 0);
		return false;
	}

	if (pClient->GetState() != SESSIONSTATE_LOGIN_WAIT)
	{
		clog::Error( clog::ERROR_PROBLEM, "ReqLogin Error!! client state error state=%d",
			pClient->GetState() );
		m_BasicProtocol.AckLogIn(senderId, SEND_T, error::ERR_INVALID_USER, id, 0);
		return false;
	}

	pClient->SetName(id);
	return true;
}


/**
 @brief ReqLogOut
 */
bool CBasicC2SHandler::ReqLogOut(IProtocolDispatcher &dispatcher, netid senderId, const std::string &id)
{

	return true;
}


/**
 @brief 
 */
bool CBasicC2SHandler::ReqMoveToServer(IProtocolDispatcher &dispatcher, netid senderId, const std::string &serverName)
{
	return true;
}


//------------------------------------------------------------------------
// groupid : -1 �̶�� root �׷��� �ڽ��� ������.
//------------------------------------------------------------------------
bool CBasicC2SHandler::ReqGroupList(IProtocolDispatcher &dispatcher, netid senderId, const netid &groupid)
{
	GroupPtr pGroup = NULL;
	if (ROOT_GROUP_NETID == groupid)
	{
		pGroup = &m_Server.GetRootGroup();
	}
	else
	{
		pGroup = m_Server.GetRootGroup().GetChildandThis(groupid);
	}

	GroupVector gv;
	if (pGroup)
	{
		const Groups::VectorType &children = pGroup->GetChildren();
		gv.reserve(children.size());
		for (u_int i=0; i < children.size(); ++i)
			gv.push_back( *children[i] );
	}
	m_BasicProtocol.AckGroupList(senderId, SEND_TARGET, 
		(pGroup)? ERR_SUCCESS : ERR_GROUPLIST_NOT_FOUND_GROUP, gv);
	return true;
}


//------------------------------------------------------------------------
// Request Joint the Group of groupid
//------------------------------------------------------------------------
bool CBasicC2SHandler::ReqGroupJoin(IProtocolDispatcher &dispatcher, netid senderId, const netid &groupid)
{
	GroupPtr pTo = (groupid == INVALID_NETID)? &m_Server.GetRootGroup() : m_Server.GetRootGroup().GetChildandThis(groupid);
	GroupPtr pFrom = m_Server.GetRootGroup().GetChildFromUser( senderId );
	if (pTo && pFrom)
	{
		if (pTo->GetId() == pFrom->GetId())
		{// Error!!
			m_BasicProtocol.AckGroupJoin( senderId, SEND_TARGET, ERR_GROUPJOIN_ALREADY_SAME_GROUP, 
				senderId, groupid );
			return false;
		}
		if (!pTo->IsTerminal())
		{// Error!!
			m_BasicProtocol.AckGroupJoin( senderId, SEND_TARGET, ERR_GROUPJOIN_NOT_TERMINAL, 
				senderId, groupid );
			return false;
		}

		pFrom->RemoveUser(pFrom->GetId(), senderId);
		pTo->AddUser(pTo->GetId(), senderId);
		m_BasicProtocol.AckGroupJoin( pTo->GetId(), SEND_T_V, ERR_SUCCESS , senderId, groupid );
		m_BasicProtocol.AckGroupJoin( pFrom->GetId(), SEND_T_V, ERR_SUCCESS , senderId, groupid );
		return true;
	}
	else
	{ // Error!!
		m_BasicProtocol.AckGroupJoin( senderId, SEND_TARGET, ERR_NOT_FOUND_GROUP, senderId, groupid );
		return false;
	}
}


//------------------------------------------------------------------------
// Request Create Group 
// �̹� group�� ������ ������ group�� �ڽ����� group�� ������ �� ����.
// ���� �̷��� �Ϸ���, group�� �Ҽӵ� ������� �� �׷쿡 �Ҽӽ�Ű��, 
// ���� group�� �ڽ����� �߰��ؾ� �Ѵ�. (�ܸ� ��忡�� ������ �Ҽӵ� �� �ִ�.)
//------------------------------------------------------------------------
bool CBasicC2SHandler::ReqGroupCreate(IProtocolDispatcher &dispatcher, netid senderId, const netid &parentGroupId, const std::string &groupName)
{
	GroupPtr pParentGroup, pFrom, pNewGroup;
	if (!CreateBlankGroup(senderId, parentGroupId, groupName, pParentGroup, pFrom, pNewGroup))
		return false;

	pFrom->RemoveUser(pFrom->GetId(), senderId);
	pNewGroup->AddUser(pNewGroup->GetId(), senderId);
	pNewGroup->AddViewer( pParentGroup->GetId() );

	const netid groupId = pNewGroup->GetId();
	m_BasicProtocol.AckGroupCreate( pNewGroup->GetId(), SEND_T_V, ERR_SUCCESS, 
		senderId, groupId, parentGroupId, groupName);
	m_BasicProtocol.AckGroupJoin( pNewGroup->GetId(), SEND_T_V, ERR_SUCCESS,
		senderId, groupId);
	return true;
}


/**
 @brief Create Blank Group
 */
bool CBasicC2SHandler::ReqGroupCreateBlank(
	IProtocolDispatcher &dispatcher, netid senderId, const netid &parentGroupId, const std::string &groupName)
{
	GroupPtr pParentGroup, pFrom, pNewGroup;
	if (!CreateBlankGroup(senderId, parentGroupId, groupName, pParentGroup, pFrom, pNewGroup))
		return false;

	pNewGroup->AddViewer( pParentGroup->GetId() );
	m_BasicProtocol.AckGroupCreateBlank( pNewGroup->GetId(), SEND_T_V, ERR_SUCCESS, 
		senderId, pNewGroup->GetId(), parentGroupId, groupName);
	return true;
}


/**
 @brief Create Blank Group
 
 �̹� group�� ������ ������ group�� �ڽ����� group�� ������ �� ����.
 ���� �̷��� �Ϸ���, group�� �Ҽӵ� ������� �� �׷쿡 �Ҽӽ�Ű��, 
 ���� group�� �ڽ����� �߰��ؾ� �Ѵ�. 
 
 �ܸ� ��忡�� �׷��� �߰��� �� ����.
 �ܸ� ��忡�� ������ �Ҽӵ� �� �ִ�. 
 */
bool	CBasicC2SHandler::CreateBlankGroup( 
	netid senderId, const netid &parentGroupId, const std::string &groupName, 
	OUT GroupPtr &pParent, OUT GroupPtr &pFrom, OUT GroupPtr &pNew )
{
	GroupPtr pParentGroup = 
		(parentGroupId == INVALID_NETID)? &m_Server.GetRootGroup() : m_Server.GetRootGroup().GetChildandThis(parentGroupId);
	if (!pParentGroup)
	{ // Error!!
		m_BasicProtocol.AckGroupCreate( senderId, SEND_TARGET, 
			ERR_GROUPCREATE_NOT_FOUND_PARENT_GROUP, senderId, 0, parentGroupId, groupName );
		return false;
	}

	//if (pParentGroup->IsTerminal())
	//{// Error!!
	//	m_BasicProtocol.AckGroupCreate( senderId, SEND_TARGET, 
	//		ERR_GROUPCREATE_PARENT_TERMINALNODE, senderId, 0, parentGroupId, groupName );
	//	return false;
	//}

	GroupPtr pFromGroup = m_Server.GetRootGroup().GetChildFromUser( senderId );
	if (!pFromGroup)
	{ // Error!!
		m_BasicProtocol.AckGroupCreate( senderId, SEND_TARGET, ERR_NOT_FOUND_USER, 
			senderId, 0, parentGroupId, groupName );
		return false;
	}

	CGroup *pNewGroup = pParentGroup->AddChild( m_Server.GetGroupFactory() );
	//CGroup *pNewGroup = new CGroup(pParentGroup, groupName);
	//const bool result = pParentGroup->AddChild( pNewGroup );
	//if (!result) 
	if (!pNewGroup)
	{ // Error!!
		if (pParentGroup->IsTerminal())
		{
				m_BasicProtocol.AckGroupCreate( senderId, SEND_TARGET, 
					ERR_GROUPCREATE_PARENT_TERMINALNODE, senderId, 0, parentGroupId, groupName );
		}
		else
		{
			m_BasicProtocol.AckGroupCreate( senderId, SEND_TARGET, ERR_GROUPCREATE_NOR_MORE_CREATE_GROUP, 
				senderId, 0, parentGroupId, groupName );
		}
		return false;
	}

	pParent = pParentGroup;
	pFrom = pFromGroup;
	pNew = pNewGroup;
	return true;
}


/**
 @brief Request peer to peer connection
 */
bool CBasicC2SHandler::ReqP2PConnect(IProtocolDispatcher &dispatcher, netid senderId)
{
	// check p2p connection
	// if networking this group on p2p
	// -> then connect to p2p host with this senderId client
	// else
	// -> select p2p host in group client, and network p2p each other

	CSession* pClient = m_Server.GetSession(senderId);
	if (!pClient)
	{
		clog::Error( clog::ERROR_PROBLEM, "not found Session netid: %d\n", senderId );
		return false;
	}

	GroupPtr pGroup = m_Server.GetRootGroup().GetChildFromUser(senderId);
	if (!pGroup)
	{
		clog::Error( clog::ERROR_PROBLEM, "not found group from user id: %d\n", senderId );
		return false;
	}

	// search up and down p2p connection
	// if already connect p2p, then this group is p2p connection group
	const netid p2pHostClient = group::GetP2PHostClient(pGroup, CServerUserAccess(&m_Server));
	if (INVALID_NETID == p2pHostClient)
	{
		const netid newHostClient = group::SelectP2PHostClient(pGroup);
		if (INVALID_NETID == newHostClient)
		{
			// error!!
			// maybe~ never happen this accident
			m_BasicProtocol.AckP2PConnect( senderId, SEND_TARGET, 
				error::ERR_P2PCONNECTION_NO_MEMBER_IN_GROUP, network::P2P_CLIENT, " ", 0 );
			return false;
		}

		// p2p host 
		// build p2p network
		pClient->SetP2PState(P2P_HOST);
		pGroup->SetNetState(CGroup::NET_STATE_P2P);
		m_BasicProtocol.AckP2PConnect( senderId, SEND_TARGET, error::ERR_SUCCESS, network::P2P_HOST, "", 2400 );
	}
	else
	{
		if (p2pHostClient == senderId)
		{
			// error!!
			m_BasicProtocol.AckP2PConnect( senderId, SEND_TARGET, 
				error::ERR_P2PCONNECTION_ALREADY_CONNECTED, network::P2P_CLIENT, "", 0);
			return false;
		}
		else
		{
			CSession *pHostClient = m_Server.GetSession(p2pHostClient);
			if (!pHostClient)
			{
				// error!!
				// maybe~ never happen this accident
				m_BasicProtocol.AckP2PConnect( senderId, SEND_TARGET, 
					error::ERR_P2PCONNECTION_HOSTCLIENT_DISAPPEAR, network::P2P_CLIENT, "", 0);
				// waitting next command. maybe good work
				return false;
			}
			else
			{
				// connect p2p client
				m_BasicProtocol.AckP2PConnect( senderId, SEND_TARGET, 
					error::ERR_SUCCESS, network::P2P_CLIENT, pHostClient->GetIp(), 2400);
			}
		}
	}

	return true;
}
