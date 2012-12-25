
#include "stdafx.h"
#include "NetLauncher.h"

using namespace network;


//------------------------------------------------------------------------
// 서버 시작
//------------------------------------------------------------------------
bool CNetLauncher::LaunchServer(CServer *pSvr, int port)
{
	if (!pSvr)
		return false;

	// 윈속을 시작하고 버전을 확인합니다
	WORD wVersionRequested = MAKEWORD(1, 1);
	WSADATA wsaData;
	int nRet = WSAStartup(wVersionRequested, &wsaData);
	if(wsaData.wVersion != wVersionRequested)
	{
		error::ErrorLog( "윈속 버전이 틀렸습니다" );
		return false;
	}

	// socket(주소계열, 소켓 형식, 프로토콜)
	SOCKET svrSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(svrSocket == INVALID_SOCKET)
	{
		error::ErrorLog( "socket() error" );
		return false;
	}

	// 주소 구조체를 채웁니다.
	SOCKADDR_IN saServer;
	saServer.sin_family = AF_INET;
	saServer.sin_addr.s_addr = INADDR_ANY;    // 윈속이 제공하게 둡니다.
	saServer.sin_port = htons(port);		// 명령줄에서 받은 포트를 넣습니다.

	// 소켓과 listensocket 을 bind(묶습) 합니다.
	// bind(소켓, 서버 주소, 주소 구조체의 길이
	nRet = bind(svrSocket, (LPSOCKADDR)&saServer, sizeof(struct sockaddr) );
	if(nRet == SOCKET_ERROR)
	{
		error::ErrorLog( "bind() error" );
		closesocket(svrSocket);
		return false;
	}

	int nLen;
	nLen = sizeof(SOCKADDR);
	char szBuf[256];

	nRet = gethostname( szBuf, sizeof(szBuf) );
	if (nRet == SOCKET_ERROR)
	{
		error::ErrorLog( "gethostname() error" );
		closesocket(svrSocket);
		return false;
	}

	// listen(오는 소켓, 요청 수용 가능한 용량)
	nRet = listen(svrSocket, SOMAXCONN);

	if (nRet == SOCKET_ERROR)
	{
		error::ErrorLog( "listen() error" );
		closesocket(svrSocket);
		return false;
	}

	pSvr->SetSocket(svrSocket);
	pSvr->SetPort(port);
	return true;
}


//------------------------------------------------------------------------
// 클라이언트 시작
//------------------------------------------------------------------------
bool CNetLauncher::LaunchClient(CClient *pClient, const std::string &ip, int port)
{
	if (!pClient)
		return false;

	pClient->SetConnect(false);

	// 윈속 버전을 확인 합니다.
	WORD wVersionRequested = MAKEWORD(1,1);
	WSADATA wsaData;
	int nRet = WSAStartup(wVersionRequested, &wsaData);
	if (wsaData.wVersion != wVersionRequested)
	{
		error::ErrorLog( "윈속 버전이 틀렸습니다" );
		return false;
	}

	LPHOSTENT lpHostEntry;
	lpHostEntry = gethostbyname(ip.c_str());
	if(lpHostEntry == NULL)
	{
		error::ErrorLog( "gethostbyname() error" );
		return false;
	}

	// TCP/IP 스트림 소켓을 생성합니다.
	// socket(주소 계열, 소켓 형태, 프로토콜
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET)
	{
		error::ErrorLog( "socket() error" );
		return false;
	}

	// 주소 구조체를 채웁니다.
	SOCKADDR_IN saServer;
	saServer.sin_family = AF_INET;
	saServer.sin_addr = *((LPIN_ADDR)*lpHostEntry->h_addr_list); // 서버 주소
	saServer.sin_port = htons(port);

	// 서버로 접속합니다
	// connect(소켓, 서버 주소, 서버 주소의 길이
	nRet = connect(clientSocket, (LPSOCKADDR)&saServer, sizeof(struct sockaddr) );
	if(nRet == SOCKET_ERROR)
	{
		error::ErrorLog( "connect() error" );
		closesocket(clientSocket);
		return false;
	}

	pClient->SetServerIp(ip);
	pClient->SetServerPort(port);
	pClient->SetSocket(clientSocket);
	pClient->SetConnect(true);
 	pClient->OnConnect();

	return true;
}
