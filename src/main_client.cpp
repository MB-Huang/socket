#include "socket.hpp"

int main()
{
    // build a client sockt
    SocketPair client(SocketPair::CLIENT);
    if ( !client.initial() )
        return 0;
    
    // set the message want to send to server
    char message[] = {"Hi there!"};
    strcpy(client.SendBuffer, message);
    
    // set I/O byte length
    client.SendLen = sizeof(message);
    client.RecvLen = 50;

    while(client.checkConnected())
    {
        client.run();
        printf("Get from server: %s\n", client.RecvBuffer);
    }

    return 0;
}