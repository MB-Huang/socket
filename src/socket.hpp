// =========================================================
//			# Compatible with Linux and Windows #
// This file include the function used to connect
// the 'client' and 'server' program with Enthernet or WiFi
// with TCP/IP socket
// =========================================================

#ifndef SOCKET_HPP
#define SOCKET_HPP

#if defined(__linux__) || defined(__APPLE__)
#include <unistd.h>	
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <sys/types.h>
#include <Winsock2.h>
#include <WS2tcpip.h>
#define	WINSOCKVERSION	MAKEWORD( 2,2 )
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// MACRO
#define SERVER_IP		"127.0.0.1"		// default server IP
#define PORT			8700			// default port number
#define RECV_BUFF_LEN	200				// defualt bytes length of receive buffer
#define SEND_BUFF_LEN	200				// defualt bytes length of send buffer

class SocketPair
{
public:
	enum SocketType { SERVER = 0, CLIENT = 1, NONE = 2 };

private:
	// socket parameter
	SocketType type;
	char *serverIP;
	int port;

	// socket state
	bool isConnected;
	bool newMsg;
	char *sendMsgContent;
	char *getMsgContent;
	int recvBytes;	// the number of received bytes

	// socket address information
	sockaddr_in serverInfo;
	sockaddr_in clientInfo;
	unsigned int server_addrlen;
	unsigned int client_addrlen;

	// the file descriptor of the socket port
	int sockfd_server;	//listening for an incoming connection
	int sockfd_client;	//operating if a connection was found

public:
	// message buffer parameter
	unsigned int RecvLen;	// bytes length of recieve data
	unsigned int SendLen;	// bytes length of send data
	char RecvBuffer[RECV_BUFF_LEN];		// buffer of recieve data
	char SendBuffer[SEND_BUFF_LEN];		// buffer of send data
	bool waitRecvComplete;	// If true, wait untill the length of received bytes as long as `RecvLen`.

	SocketPair(SocketType type);
	~SocketPair();

	// Setup
	void setServerIP(const char *sIP){strcpy(serverIP, sIP);}
	void setPortNum(const int &P){this->port=P;}
	bool checkConnected(void){return isConnected;}

	bool initial(void);
	void run(void);
	void closeSocket(void);

private:
	// Initialize
	bool initialServer(void);
	bool initialClient(void);
	// Runing
	void runServer(void);
	void runClient(void);
};

#endif // !SOCKET_HPP