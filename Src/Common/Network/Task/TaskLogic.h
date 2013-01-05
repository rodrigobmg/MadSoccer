//------------------------------------------------------------------------
// Name:    TaskLogic.h
// Author:  jjuiddong
// Date:    12/22/2012
// 
// ��Ʈ��ũ�κ��� ���� ��Ŷ�� �Ľ��ؼ� Listener�� ��ϵ� ��ü���� �޼�����
// ������.
//------------------------------------------------------------------------
#pragma once

#include "../DataStructure/PacketQueue.h"
#include "../Controller/NetController.h"
#include "../Service/AllProtocolListener.h"


namespace network
{
	class CTaskLogic : public common::CTask
	{
	public:
		CTaskLogic() : CTask(0) {}

	public:
		virtual RUN_RESULT	Run() override
		{
			CPacketQueue::SPacketData packetData;
			if (!CPacketQueue::Get()->PopPacket(packetData))
				return RR_CONTINUE;

			CServer *pSvr = GetServer(packetData.rcvServerSock);
			if (!pSvr)
			{
				error::ErrorLog( 
					common::format("CTaskLogic:: %d �� �ش��ϴ� ������ �����ϴ�.", 
						packetData.rcvServerSock) );
				return RR_CONTINUE;
			}

 			const ProtocolListenerList &listeners = pSvr->GetListeners();
			if (listeners.empty())
			{
				error::ErrorLog( 
					common::format("CTaskLogic %d NetConnector�� �������� �����ʰ� �����ϴ�.", 
					pSvr->GetNetId()) );
				return RR_CONTINUE;
			}

			// ��� ��Ŷ�� �޾Ƽ� ó���ϴ� �����ʿ��� ��Ŷ�� ������.
			all::Dispatcher allDispatcher;
			allDispatcher.Dispatch(packetData.packet, listeners);
			// 

			const int protocolId = packetData.packet.GetProtocolId();
			IProtocolDispatcher *pDispatcher = CNetController::Get()->GetDispatcher(protocolId);
			if (!pDispatcher)
			{
				error::ErrorLog( 
					common::format("CTaskLogic:: %d �� �ش��ϴ� �������� �����İ� �����ϴ�.", 
					protocolId) );
				return RR_CONTINUE;
			}

			pDispatcher->Dispatch(packetData.packet, listeners);

			return RR_CONTINUE;
		}
	};
}