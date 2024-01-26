#include "NetSystem.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include "../Core/ErrorWarningAssert.hpp"
#include "../Core/DevConsole.hpp"


#if !defined( ENGINE_DISABLE_NETWORKING )


extern NetSystem* g_theNet;
extern DevConsole* g_theDevConsole;

NetSystem::NetSystem(const NetSystemConfig& config)
	:m_config(config)
{
	if (m_config.m_modeString == "Server")
	{
		m_mode = Mode::SERVER;
	}
	else if (m_config.m_modeString == "Client")
	{
		m_mode = Mode::CLIENT;
	}
	else
	{
		m_mode = Mode::NONE;
	}

	m_sendBuffer = new char[m_config.m_sendBufferSize];
	m_recvBuffer = new char[m_config.m_recvBufferSize];
	
}

NetSystem::~NetSystem()
{
	delete m_sendBuffer;
	delete m_recvBuffer;
	m_sendQueue.clear();
	m_recvRemaining.clear();
}

void NetSystem::Startup()
{
	int result;
	unsigned long blockingMode = 1;

	SubscribeEventCallbackFunction("RemoteCommand", Event_RemoteCommand);
	SubscribeEventCallbackFunction("BurstTest", Event_BurstTest);
	switch (m_mode)
	{
	case Mode::NONE:
		
		break;
	case Mode::CLIENT:

		WSADATA data;
		result = WSAStartup(MAKEWORD(2, 2), &data);

		m_clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		blockingMode = 1;
		result = ioctlsocket(m_clientSocket, FIONBIO, &blockingMode);

		IN_ADDR addrClient;
		result = inet_pton(AF_INET, m_config.m_hostAddressString.c_str(), &addrClient);
		m_hostAddress = ntohl(addrClient.S_un.S_addr);


		m_hostPort = (unsigned short)(atoi(m_config.m_hostPort.c_str()));
		m_clientState = ClientState::READY_TO_CONNECT;
		break;
	case Mode::SERVER:
		result = WSAStartup(MAKEWORD(2, 2), &data);
		m_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		blockingMode = 1;
		result = ioctlsocket(m_listenSocket, FIONBIO, &blockingMode);

		m_hostAddress = INADDR_ANY;
		m_hostPort = (unsigned short)(atoi(m_config.m_hostPort.c_str()));

		sockaddr_in addrServer;
		addrServer.sin_family = AF_INET;
		addrServer.sin_addr.S_un.S_addr = htonl(m_hostAddress);
		addrServer.sin_port = htons(m_hostPort);
		result = bind(m_listenSocket, (sockaddr*)&addrServer, (int)sizeof(addrServer));


		result = listen(m_listenSocket, SOMAXCONN);
		m_serverState = ServerState::LISTENING;
		break;
	}
	
}

void NetSystem::Shutdown()
{
	switch (m_mode)
	{
	case Mode::NONE:
		break;
	case Mode::CLIENT:

		closesocket(m_clientSocket);
		WSACleanup();

		break;
	case Mode::SERVER:

		closesocket(m_clientSocket);
		closesocket(m_listenSocket);
		WSACleanup();

		break;
	}
	
	
}

void NetSystem::BeginFrame()
{
	int result;
	unsigned long blockingMode = 1;
	sockaddr_in addr;
	fd_set successSockets;
	FD_ZERO(&successSockets);
	timeval successWaitTime = { };
	fd_set failedSockets;
	FD_ZERO(&failedSockets);
	timeval failedWaitTime = { };
	switch (m_mode)
	{
	case Mode::NONE:
		break;
	case Mode::CLIENT:
		switch (m_clientState)
		{
		case NetSystem::ClientState::INVALID:
			break;
		case NetSystem::ClientState::READY_TO_CONNECT:

			addr.sin_family = AF_INET;
			addr.sin_addr.S_un.S_addr = htonl(m_hostAddress);
			addr.sin_port = htons(m_hostPort);
			result = connect(m_clientSocket, (sockaddr*)(&addr), (int)sizeof(addr));
			if (result == SOCKET_ERROR)
			{
				int error = WSAGetLastError();
				if (error != WSAEWOULDBLOCK)
				{
					ERROR_AND_DIE("Error happened at Connect()\n");
				}
				else
				{
					m_clientState = ClientState::CONNECTING;
				}
			}
			break;
		case NetSystem::ClientState::CONNECTING:

			
			FD_SET(m_clientSocket, &failedSockets);

			result = select(0, NULL, NULL, &failedSockets, &failedWaitTime);

			if (result == SOCKET_ERROR)
			{
				ERROR_AND_DIE("Error happened at Select()1\n");
			}
			if (result > 0 && FD_ISSET(m_clientSocket, &failedSockets))
			{
				m_clientState = ClientState::READY_TO_CONNECT;
				return;
			}

			//Check if the connection attempt completed.
			
			FD_SET(m_clientSocket, &successSockets);
	
			result = select(0, NULL, &successSockets, NULL, &successWaitTime);


			if (result > 0 && FD_ISSET(m_clientSocket, &successSockets))
			{
				m_clientState = ClientState::CONNECTED;
				g_theDevConsole->AddLine(Rgba8::RED, "CONNECTED!");
			}



			break;
		case NetSystem::ClientState::CONNECTED:

			result = recv(m_clientSocket, m_recvBuffer, m_config.m_recvBufferSize, 0);
			if (result == -1)
			{
				int error = WSAGetLastError();
				if (error == WSAECONNRESET)
				{
					m_clientState = ClientState::READY_TO_CONNECT;
					Shutdown();
					Startup();
					return;
				}
				else if (error != WSAEWOULDBLOCK)
				{
					ERROR_AND_DIE("Error happened at Recv() Function\n");
				}
			}
			else if (result == 0)  //connection has shut down close our end nicely
			{
				m_clientState = ClientState::READY_TO_CONNECT;
				Shutdown();
				Startup();
				return;
			}

			for (int i = 0; i < (int)m_sendQueue.size(); ++i)
			{
				if (m_config.m_sendBufferSize < sizeof(m_sendQueue[i]))
				{
					ERROR_AND_DIE("Size of sendBuffer is too small");
				}
				if (!strncpy_s(m_sendBuffer, m_config.m_sendBufferSize, m_sendQueue[i].c_str(), strlen(m_sendQueue[i].c_str()) + 1) == 0)
				{
					ERROR_AND_DIE("copy failed");
				}

				int sendResult = send(m_clientSocket, m_sendBuffer, (int)strlen(m_sendBuffer) + 1, 0);
				if (sendResult == SOCKET_ERROR)
				{
					int error = WSAGetLastError();
					if (error != WSAEWOULDBLOCK)
					{
						ERROR_AND_DIE("Error happened at send() Function\n");
					}
				}
			}


			m_sendQueue.clear();
			for (int recvCharIndex = 0; recvCharIndex < result; ++recvCharIndex)
			{
				char tempChar = m_recvBuffer[recvCharIndex];
				if (tempChar > 0)
				{
					m_recvRemaining += tempChar;
				}
				else
				{
					if (!m_recvRemaining.empty())
					{
						g_theDevConsole->Execute(m_recvRemaining, true);
						m_recvRemaining.clear();
					}
					continue;
				}
			}

			break;
		default:
			break;
		}

		break;
	case Mode::SERVER:

		switch (m_serverState)
		{
		case NetSystem::ServerState::INVALID:
			break;
		case NetSystem::ServerState::LISTENING:
			if (m_clientSocket == ~0ull)
			{
				uintptr_t socket = accept(m_listenSocket, NULL, NULL);
				m_clientSocket = socket;
			}
			else
			{
				blockingMode = 1;
				result = ioctlsocket(m_clientSocket, FIONBIO, &blockingMode);
				m_serverState = ServerState::CONNECTED;
			}

			break;
		case NetSystem::ServerState::CONNECTED:

			result = recv(m_clientSocket, m_recvBuffer, m_config.m_recvBufferSize, 0);
			if (result == SOCKET_ERROR)
			{
				int error = WSAGetLastError();
				if (error == WSAECONNRESET)
				{
					m_serverState = ServerState::LISTENING;
					Shutdown();
					Startup();
					return;
				}
				else if (error != WSAEWOULDBLOCK)
				{
					DebuggerPrintf("Error happened at recv() Function\n");
				}
			}
			else if (result == 0)
			{
				m_serverState = ServerState::LISTENING;
				Shutdown();
				Startup();
				return;
			}

			for (int i = 0; i < (int)m_sendQueue.size(); ++i)
			{

				if (!strncpy_s(m_sendBuffer, m_config.m_sendBufferSize, m_sendQueue[i].c_str(), strlen(m_sendQueue[i].c_str()) + 1) == 0)
					//Copy failed if true
				{
					ERROR_AND_DIE("copy failed");
				}
				if (result > m_config.m_recvBufferSize)
				{
					ERROR_AND_DIE("too Big");
				}
				int sendResult = send(m_clientSocket, m_sendBuffer, (int)strlen(m_sendBuffer) + 1, 0);
				if (sendResult == SOCKET_ERROR)
				{
					int error = WSAGetLastError();
					if (error != WSAEWOULDBLOCK)
					{
						ERROR_AND_DIE("Error happened at send() Function\n");
					}
				}
			}

			m_sendQueue.clear();

			for (int recvCharIndex = 0; recvCharIndex < result; ++recvCharIndex)
			{
				char const& tempChar = m_recvBuffer[recvCharIndex];
				if (tempChar == -51)
				{
					break;
				}
				if (tempChar != '\0')
				{
					m_recvRemaining += tempChar;
				}
				else
				{
					if (!m_recvRemaining.empty())
					{
						g_theDevConsole->Execute(m_recvRemaining, true);
						std::string const ackBegin = "Server Executed Remote Command: ";
						std::string const acknowledgement = ackBegin + m_recvRemaining;
						m_sendQueue.push_back(acknowledgement);
						m_recvRemaining.clear();
					}
					continue;
				}
			}
			
			break;
		default:
			break;
		}

		break;
	}

}

void NetSystem::EndFrame()
{

}

bool NetSystem::Event_RemoteCommand(EventArgs& args)
{
	std::string cmd;
	cmd = args.GetValue("Command", cmd);
	TrimString(cmd, '\"');
	g_theNet->m_sendQueue.push_back(cmd);
	return true;
}


bool NetSystem::Event_BurstTest(EventArgs& args)
{
	UNUSED(args);
	for (int index = 1; index <= 20; ++index)
	{
		g_theNet->m_sendQueue.emplace_back(Stringf("Echo Message=%d", index));
	}
	return true;
}

#endif // !defined( ENGINE_DISABLE_NETWORKING )