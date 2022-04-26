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
    int clientSocketDescriptor, socketDescriptor, toSend=0;
    int *toSendPtr;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientLength = sizeof(clientAddress);
    char recvBuffer[1000], sendBuffer[1000],temp[1000];
    char *sendBufferPtr;
    pid_t cpid,cpid2,cpid1;

    toSendPtr = &toSend;
    sendBufferPtr = sendBuffer;

    bzero(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(1234);
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    bind(socketDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    listen(socketDescriptor, 10);

    printf("\nServer is running ...\n");

    cpid = fork();
    if (!cpid)
    {
        while(1)
        {
            sleep(1);
            //*toSendPtr = 0;
            bzero(temp,sizeof(temp));
            printf("\nType message for client(s): ..." );

            /*Read the message from client*/
            fflush(stdin);
            fgets(temp,10000,stdin);
            strcpy(sendBufferPtr,temp);
            *toSendPtr = 1;
        }
    } else
    {
      while (1)
      {
        clientSocketDescriptor = accept(socketDescriptor, (struct sockaddr *)&clientAddress, &clientLength);
        cpid1 = fork();
        if (!cpid1)
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
                        kill(getpid()+1, SIGQUIT);
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
                    if (*toSendPtr == 1)
                    {
                        /*Sends the message to client*/
                        strcpy(temp,sendBufferPtr);
                        // temp = sendBufferPtr;
                        send(clientSocketDescriptor,temp,strlen(temp)+1,0);
                        printf ("\n *tosendptr , %i \n",*toSendPtr);
                        printf ("\n temp , %s \n",temp);
                        printf("\nMessage sent to client!");
                        sleep(1);
                        *toSendPtr=0;
                    }

                }
            }
        }
      }
    }
    return 0;
}
