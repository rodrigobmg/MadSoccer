//------------------------------------------------------------------------
// Name:    ServerBasic.h
// Author:  jjuiddong
// Date:    2012-11-27
// 
// 클라이언트가 접속할 Server를 표현한다.
//------------------------------------------------------------------------
#pragma once

#include "../interface/ServerEventListener.h"

namespace network
{
	class IServerEventListener;
	class CServerBasic : public CNetConnector
	{
		friend class CNetLauncher;
		friend class CNetController;
		friend class CTaskLogic;
		friend class CP2PClient;
		typedef common::ReferencePtr<IServerEventListener> ServerEventListenerPtr;

	public:
		CServerBasic(PROCESS_TYPE procType);
		virtual ~CServerBasic();

		bool				AddRemoteClient(SOCKET sock, const std::string &ip);
		CRemoteClient* GetRemoteClient(netid netId);
		bool				RemoveRemoteClient(netid netId);
		bool				RemoveRemoteClientSocket(netid netId);
		RemoteClientItor	RemoveRemoteClientInLoop(netid netId);
		netid			GetNetIdFromSocket(SOCKET sock);
		bool				IsExist(netid netId);
		void				Clear();

		void				Proc();
		bool				Stop();
		void				Disconnect();
		void				Close();
		bool				IsServerOn() const;
		void				SetEventListener(ServerEventListenerPtr ptr) { m_pEventListener = ptr; }
		void				MakeFDSET( SFd_Set *pfdset);
		void				SetPort(int port);

		virtual bool	Send(netid netId, const SEND_FLAG flag, const CPacket &packet) override;
		virtual bool	SendAll(const CPacket &packet) override;

		// Event Handler
		void				OnListen();
		void				OnClientJoin(netid netId);
		void				OnClientLeave(netid netId);

	protected:
		bool				AcceptProcess();
		void				DispatchPacket();
		bool				SendGroup(GroupPtr pGroup, const CPacket &packet);
		bool				SendViewer(netid groupId, const SEND_FLAG flag, const CPacket &packet);
		bool				SendViewerRecursive(netid viewerId, const netid exceptGroupId, const CPacket &packet);

		RemoteClientItor	RemoveClientProcess(RemoteClientItor it);
		RemoteClientItor	FindRemoteClientBySocket(SOCKET sock);

	protected:
		int								m_ServerPort;
		bool								m_IsServerOn;			// 서버가 정상적으로 실행이 되었다면 true
		RemoteClientMap		m_RemoteClients;		// 서버와 연결된 클라이언트 정보리스트
		common::CriticalSection  m_CS;
		ServerEventListenerPtr m_pEventListener;

		CGroup							m_RootGroup;
		netid							m_WaitGroupId;			// waiting place before join concrete group
	};

	inline bool CServerBasic::IsServerOn() const { return m_IsServerOn; }
	inline void	 CServerBasic::SetPort(int port) { m_ServerPort = port; }

};
