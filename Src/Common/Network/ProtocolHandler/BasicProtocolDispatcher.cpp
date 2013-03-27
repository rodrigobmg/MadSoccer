
#include "stdafx.h"
#include "BasicProtocolDispatcher.h"
#include "Network/Controller/NetController.h"
#include "network/PrtCompiler/ProtocolMacro.h"
#include "../Controller/CoreClient.h"


using namespace network;
using namespace basic_protocol;

//------------------------------------------------------------------------
// ��Ŷ�� �������ݿ� ���� �ش��ϴ� �������� �Լ��� ȣ���Ѵ�.
//------------------------------------------------------------------------
void basic_protocol::ServerDispatcher::Dispatch(CPacket &packet, ServerBasicPtr pSvr)
{
	RET(!pSvr);

	CPacket newPacket = packet;
	switch (newPacket.GetPacketId())
	{
	case PACKETID_DISCONNECT:
		{
			int uniqueValue = 0;
			newPacket >> uniqueValue;
			if (CNetController::Get()->GetUniqueValue() != uniqueValue) // ��Ŷ ����
				return;
				 
			netid disconnectId = INVALID_NETID;
			newPacket >> disconnectId;
			if (pSvr->GetNetId() == disconnectId)
			{
				pSvr->Disconnect();
			}
			else
			{
				pSvr->RemoveRemoteClient(disconnectId);
			}
		}
		break;
	}
}


//------------------------------------------------------------------------
// ��Ŷ�� �������ݿ� ���� �ش��ϴ� �������� �Լ��� ȣ���Ѵ�.
//------------------------------------------------------------------------
void basic_protocol::ClientDispatcher::Dispatch(CPacket &packet, CoreClientPtr pClt)
{
	RET(!pClt);

	CPacket newPacket = packet;
	switch (newPacket.GetPacketId())
	{
	case PACKETID_DISCONNECT:
		{
			int uniqueValue = 0;
			newPacket >> uniqueValue;
			if (CNetController::Get()->GetUniqueValue() != uniqueValue) // ��Ŷ ����
				return;

			pClt->Disconnect();
		}
		break;
	}
}