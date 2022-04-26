#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char *argv[])
{
    int clientSocketDescriptor, socketDescriptor;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientLength = sizeof(clientAddress);
    char recvBuffer[1000], sendBuffer[1000];
    pid_t cpid,cpid2;
    bzero(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(1234);
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    bind(socketDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    listen(socketDescriptor, 10);
    printf("\nServer is running ...\n");

    while (1)
    {
        clientSocketDescriptor = accept(socketDescriptor, (struct sockaddr *)&clientAddress, &clientLength);
        cpid = fork();
        if (!cpid)
        {
            cpid2 = fork();
            if (!cpid2)
            {
                while(1)
                {
                    bzero(&recvBuffer, sizeof(recvBuffer));
                    //Receiving the request from client
                    recv(clientSocketDescriptor, recvBuffer, sizeof(recvBuffer), 0);
                    if (strcmp(recvBuffer, "END\n") == 0)
                    {
                        printf("\n Client at %s port %i DISCONNECTED \n", inet_ntoa(clientAddress.sin_addr), htons(clientAddress.sin_port));
                        close(clientSocketDescriptor);
                        kill(getpid()-1, SIGQUIT);
                        kill(getpid(), SIGQUIT);
                    } else
                    {
                        printf("\nClient at %s port %i: %s", inet_ntoa(clientAddress.sin_addr), htons(clientAddress.sin_port),recvBuffer);
                    }
                }
            } else
            {
                while(1)
                {
                    bzero(&sendBuffer,sizeof(sendBuffer));
                    printf("\nType message for client: ...");

                    /*Read the message from client*/
                    fflush(stdin);
                    fgets(sendBuffer,10000,stdin);

                    /*Sends the message to client*/

                    send(clientSocketDescriptor,sendBuffer,strlen(sendBuffer)+1,0);
                    printf("\nMessage sent to client!");
                }
            }
        }
    }
    return 0;
}
