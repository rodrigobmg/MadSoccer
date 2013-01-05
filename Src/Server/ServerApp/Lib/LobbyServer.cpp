
#include "stdafx.h"
#include "LobbyServer.h"
#include <boost/smart_ptr.hpp>

using namespace network;

CLobbyServer::CLobbyServer()
{
	RegisterProtocol(&m_LoginProtocol);
	RegisterProtocol(&m_BasicProtocol);
	AddListener( this );

}

CLobbyServer::~CLobbyServer() 
{

}


//------------------------------------------------------------------------
// ���� �߰�
//------------------------------------------------------------------------
bool CLobbyServer::AddUser(CUser *pUser)
{
	UserItor it = m_Users.find(pUser->GetNetId());
	if (m_Users.end() != it)
		return false; // �̹� �����Ѵٸ� ����
	m_Users.insert( UserMap::value_type(pUser->GetNetId(), pUser) );
	return true;
}


//------------------------------------------------------------------------
// ���� ����
// ���ڷ� �Ѿ�� pUser �޸𸮸� �����Ѵ�.
//------------------------------------------------------------------------
bool CLobbyServer::RemoveUser(CUser *pUser)
{
	UserItor it = m_Users.find(pUser->GetNetId());
	if (m_Users.end() == it)
		return false; // ���ٸ� ����
	delete pUser;
	m_Users.erase(it);
	return true;
}


//------------------------------------------------------------------------
// ���� ����
// �޸𸮱��� ���ŵȴ�.
//------------------------------------------------------------------------
bool CLobbyServer::RemoveUser(netid netId)
{
	UserItor it = m_Users.find(netId);
	if (m_Users.end() == it)
		return false; // ���ٸ� ����
	delete it->second;
	m_Users.erase(it);
	return true;
}


//------------------------------------------------------------------------
// ���� ���
//------------------------------------------------------------------------
UserPtr	CLobbyServer::GetUser(netid netId)
{
	UserItor it = m_Users.find(netId);
	if (m_Users.end() == it)
		return NULL; // ���ٸ� ����
	return it->second;
}


//------------------------------------------------------------------------
// �� �߰�
//------------------------------------------------------------------------
bool CLobbyServer::AddRoom(CRoom *pRoom)
{
	RoomItor it = m_Rooms.find(pRoom->GetId());
	if (m_Rooms.end() != it)
		return false; // �̹� �����Ѵٸ� ����
	m_Rooms.insert( RoomMap::value_type(pRoom->GetId(), pRoom) );
	return true;
}


//------------------------------------------------------------------------
// ������
// �޸𸮱��� ���ŵȴ�.
//------------------------------------------------------------------------
bool CLobbyServer::RemoveRoom(CRoom *pRoom)
{
	RoomItor it = m_Rooms.find(pRoom->GetId());
	if (m_Rooms.end() == it)
		return false; // ���ٸ� ����
	delete it->second;
	m_Rooms.erase(it);
	return true;
}


//------------------------------------------------------------------------
// ������
// �޸𸮱��� ���ŵȴ�.
//------------------------------------------------------------------------
bool CLobbyServer::RemoveRoom(int roomId)
{
	RoomItor it = m_Rooms.find(roomId);
	if (m_Rooms.end() == it)
		return false; // ���ٸ� ����
	delete it->second;
	m_Rooms.erase(it);
	return true;
}


//------------------------------------------------------------------------
// ������ ���
//------------------------------------------------------------------------
RoomPtr CLobbyServer::GetRoom(int roomId)
{
	RoomItor it = m_Rooms.find(roomId);
	if (m_Rooms.end() == it)
		return NULL; // ���ٸ� ����
	return it->second;
}


//------------------------------------------------------------------------
// �������� userId Ŭ���̾�Ʈ���� ������.
//------------------------------------------------------------------------
void CLobbyServer::SendRooms(netid userId)
{
	BOOST_FOREACH(RoomMap::value_type &vk, m_Rooms)
	{

	}
}


//------------------------------------------------------------------------
// ���������� userId Ŭ���̾�Ʈ���� ������.
//------------------------------------------------------------------------
void CLobbyServer::SendUsers(netid userId)
{

}


//------------------------------------------------------------------------
// Ŭ���̾�Ʈ�� ������ �پ��� �� ȣ��ȴ�.
//------------------------------------------------------------------------
void CLobbyServer::OnClientJoin(netid netId)
{

}


//------------------------------------------------------------------------
// Ŭ���̾�Ʈ�� ������ ���� ������ ������ �� ȣ��ȴ�.
//------------------------------------------------------------------------
void CLobbyServer::OnClientLeave(netid netId)
{

}