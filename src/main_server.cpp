#include "socket.hpp"

int main()
{
    // build a server socket
    SocketPair server(SocketPair::SERVER);
    if ( !server.initial() )
        return 0;
    
    // set the message want to send to client
    char message[] = {"Hi, this is server."};
    strcpy(server.SendBuffer, message);

    // set I/O byte length
    server.SendLen = sizeof(message);
    server.RecvLen = 50;

    while(server.checkConnected())
    {
        server.run();
        printf("Get from client: %s\n", server.RecvBuffer);
    }

    return 0;
}