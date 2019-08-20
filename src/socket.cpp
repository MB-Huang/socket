#include "socket.hpp"

SocketPair::SocketPair(SocketType type) : type(type)
{
	serverIP = (char*)malloc(15*sizeof(char));	// allocate the memory to char *
	strcpy(serverIP, SERVER_IP);
	port = PORT;
	waitRecvComplete = false;

	isConnected = false;
	newMsg = false;

	sockfd_server = 0;
	sockfd_client = 0;
	RecvLen = 0;
	SendLen = 0;
	RecvBuffer = nullptr;
	SendBuffer = nullptr;
	setRecvBuffer(RECV_BUFF_LEN);
	setSendBuffer(SEND_BUFF_LEN);
}

SocketPair::~SocketPair()
{
	free(this->serverIP);	// release memory
	free(this->RecvBuffer);
	free(this->SendBuffer);
	RecvBuffer = nullptr;	// label as invalid pointer
	SendBuffer = nullptr;
	this->closeSocket();
}

void SocketPair::setRecvBuffer(size_t len)
{
	if (RecvBuffer != nullptr)
		free(this->RecvBuffer);
	RecvBuffer = (char*)malloc(len);
}

void SocketPair::setSendBuffer(size_t len)
{
	if (SendBuffer != nullptr)
		free(this->SendBuffer);
	SendBuffer = (char*)malloc(len);
}

bool SocketPair::initial(void)
{
	switch (this->type)
	{
	case SERVER:
		return this->initialServer();
		break;
	case CLIENT:
		return this->initialClient();
		break;
	case NONE:
	default:
		printf("please choose the socket type (server or client)");
		return false;
		break;
	}
}

void SocketPair::run(void)
{
	switch (this->type)
	{
	case SERVER:
		this->runServer();
		break;
	case CLIENT:
		this->runClient();
		break;
	case NONE:
	default:
		printf("please choose the socket type (server or client)");
		break;
	}
}

void SocketPair::closeSocket()
{
	// close socket	
	if (isConnected)
	{
		switch (this->type)
		{
		case SERVER:
#if defined(__linux__) || defined(__APPLE__)
			close(sockfd_server);
#elif defined(_WIN32) || defined(_WIN64)
			closesocket(sockfd_server);
#endif
			printf("close server\n");
			break;

		case CLIENT:
#if defined(__linux__) || defined(__APPLE__)
			close(sockfd_client);
#elif defined(_WIN32) || defined(_WIN64)
			closesocket(sockfd_client);
#endif
			printf("close client\n");
			break;
		case NONE:
		default:
			printf("please choose the socket type (server or client)");
			break;
		}
	}

	// update the flag
	newMsg = false;
	isConnected = false;
}

// ----------------------------------------------------------------- //

bool SocketPair::initialServer()
{
#if defined(_WIN32) || defined(_WIN64)
	// Start up WinSock2
	WSADATA wsaData;
	if (WSAStartup(WINSOCKVERSION, &wsaData) != 0)
		return false;
#endif

	/* Create the server socket */
	// AF_INET: shows that the cocket belong to 'internet family'
	// SOCK_STREAM: shows the socket is 'connection-oriented socket'
	sockfd_server = socket(AF_INET , SOCK_STREAM , 0);
    if (sockfd_server == -1)
	{
		printf("[Socket Error] Fail to create a server socket.\n");
		return false;
	}

	/* Setup the server socket information and server IP address */
	server_addrlen = sizeof(serverInfo);
#if defined(__linux__) || defined(__APPLE__)
	bzero(&serverInfo, server_addrlen);
#elif defined(_WIN32) || defined(_WIN64)
	memset(&serverInfo, 0, server_addrlen);
#endif
    serverInfo.sin_family = AF_INET;
	serverInfo.sin_port = htons(port);
    serverInfo.sin_addr.s_addr = INADDR_ANY; // set server address

    /* assign a port number to socket */
    if ( bind(sockfd_server, (struct sockaddr *)&serverInfo, server_addrlen) )
	{
		printf("[Socket Error] Fail to bind the server\n");
		// close server socket
#if defined(__linux__) || defined(__APPLE__)
		close(sockfd_server);
#elif defined(_WIN32) || defined(_WIN64)
		closesocket(sockfd_server);
#endif
		return false;
	}

	/* Setup the number to listen to, and start listening */
	listen(sockfd_server, SOMAXCONN);

	/* Wait and accpet the connection of socket */
	/* The program would stop here untill the client connect to the server */
	client_addrlen = sizeof(clientInfo);
	sockfd_client = accept(sockfd_server,(struct sockaddr*) &clientInfo, &client_addrlen);

	isConnected = true;
	return true;
}

void SocketPair::runServer()
{
	try
	{
		/* send the message to client */
		send(sockfd_client, SendBuffer, SendLen, 0);

		/* clean the buffer */
		memset(RecvBuffer, 0, RecvLen);
		recvBytes = 0;

		if (this->waitRecvComplete)
		{
			/* receive the message from client and check the data compelete */
			for (int i = 0; i < RecvLen; i += recvBytes) 
			{
				recvBytes = recv(sockfd_client, RecvBuffer + i, RecvLen - i, 0);

				// check the received bytes number
				if ( recvBytes > 0 )
				{	// get new message
					newMsg = true;
				}
				else if ( recvBytes == 0 )
				{	// can't get the message (the bytes number is zero)
					printf("[Socket Error] Can't get the data from client\n");
					throw recvBytes;	// throw exception
				}
				else
				{	// weird result
					printf("[Socket Error] Server recv failed\n");
					throw recvBytes;	// throw exception
				}
			}
		}
		else
		{
			recv(sockfd_client, RecvBuffer, RecvLen, 0);
		}
	}

	// catch the exception
	catch ( ... )
	{
		isConnected = false;
		newMsg = false;

		// close socket
#if defined(__linux__) || defined(__APPLE__)
		close(sockfd_client);
		close(sockfd_server);
#elif defined(_WIN32) || defined(_WIN64)
		closesocket(sockfd_client);
		closesocket(sockfd_server);
#endif

		// initial and wait client again
		this->initialServer();
	}
}


bool SocketPair::initialClient()
{
#if defined(_WIN32) || defined(_WIN64)
	// Start up WinSock2
	WSADATA wsaData;
	if (WSAStartup(WINSOCKVERSION, &wsaData) != 0)
		return false;
#endif

	// Create the client socket
	// AF_INET: shows that the cocket belong to 'internet family'
	// SOCK_STREAM: shows the socket is 'connection-oriented socket'
	sockfd_client = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd_client == -1)
	{
		printf("[Socket Error] Fail to create a client socket.");
		return false;
	}

	/* Setup the server socket information and server IP address */
	server_addrlen = sizeof(serverInfo);
#if defined(__linux__) || defined(__APPLE__)
	bzero(&serverInfo, server_addrlen);
#elif defined(_WIN32) || defined(_WIN64)
	memset(&serverInfo, 0, server_addrlen);
#endif
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_port = htons(port);
	serverInfo.sin_addr.s_addr = inet_addr(serverIP); // set server address

	/* Setup the client socket information and server IP address */
	client_addrlen = sizeof(clientInfo);
#if defined(__linux__) || defined(__APPLE__)
	bzero(&clientInfo, client_addrlen);
#elif defined(_WIN32) || defined(_WIN64)
	memset(&clientInfo, 0, client_addrlen);
#endif
	clientInfo.sin_family = AF_INET;
	clientInfo.sin_port = htons(0);
	clientInfo.sin_addr.s_addr = htonl(INADDR_ANY);

	/* assign a port number to client socket */
	if (bind(sockfd_client, (struct sockaddr *)&clientInfo, client_addrlen))
	{
		printf("[Socket Error] Fail to bind the server\n");
		return false;
	}

	/* Connect to the server */
	int err = connect(sockfd_client,(struct sockaddr *)&serverInfo, server_addrlen);
	if ( err == -1 )
	{	/* Check if the connection success */
		printf("[Socket Error] Client socket cannot connect to server\n");
		return false;
	}
	else
	{
		isConnected = true;
		return true;
	}
}

void SocketPair::runClient()
{
	try
	{
		/* send the message to server */
		send(sockfd_client, SendBuffer, SendLen, 0);

		/* clean the buffer */
		memset(RecvBuffer, 0, RecvLen);
		recvBytes = 0;	// initial

		if (this->waitRecvComplete)
		{
			/* receive the message from server and check the data compelete */
			for (int i = 0; i < RecvLen; i += recvBytes) 
			{
				recvBytes = recv(sockfd_client, RecvBuffer + i, RecvLen - i, 0);

				// check the received bytes number
				if ( recvBytes > 0 )
				{	// get new message
					newMsg = true;
				}
				else if ( recvBytes == 0 )
				{	// can't get the message (the bytes number is zero)
					printf("[Socket Error] Can't get the data from server\n");
					throw recvBytes;	// throw exception
				}
				else
				{	// weird result
					printf("[Socket Error] Client recv failed\n");
					throw recvBytes;	// throw exception
				}
			}
		}
		else
		{
			recv(sockfd_client, RecvBuffer, RecvLen, 0);
		}
	}

	/* catch the exception */
	catch (...)
	{
		isConnected = false;
		newMsg = false;

		// close client socket
#if defined(__linux__) || defined(__APPLE__)
		close(sockfd_client);
#elif defined(_WIN32) || defined(_WIN64)
		closesocket(sockfd_client);
#endif

		// initial the client and connect to server again
		this->initialClient();
	}
}