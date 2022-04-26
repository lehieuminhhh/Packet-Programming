#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/signal.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<time.h>

int main(int argc,char *argv[])
{
    int clientSocketDescriptor,socketDescriptor,count;
    FILE *log;
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    struct sockaddr_in serverAddress,clientAddress;
    socklen_t clientLength;

    char recvBuffer[1000],sendBuffer[1000];
    pid_t cpid;
    bzero(&serverAddress,sizeof(serverAddress));

    /*Socket address structure*/
    serverAddress.sin_family=AF_INET;
    serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
    serverAddress.sin_port=htons(5500);

    /*TCP socket is created, an Internet socket address structure is filled with
    wildcard address & server’s well known port*/
    socketDescriptor=socket(AF_INET,SOCK_STREAM,0);

    /*Bind function assigns a local protocol address to the socket*/
    bind(socketDescriptor,(struct sockaddr*)&serverAddress,sizeof(serverAddress));

    /*Listen function specifies the maximum number of connections that kernel should queue
    for this socket*/
    listen(socketDescriptor,5);
    printf("%s\n","Server is running ...");

    /*The server to return the next completed connection from the front of the
    completed connection Queue calls it*/

    while(1)
    {
        clientSocketDescriptor=accept(socketDescriptor,(struct sockaddr*)&clientAddress,&clientLength);
        log = fopen("chatlog.txt","a");
        printf ("connected by %s %d \n",inet_ntoa(clientAddress.sin_addr),htons(clientAddress.sin_port));
        fprintf (log,"%s    Client at port %i connected.\n\n",asctime(tm),5500);
        break;
    }

    /*Fork system call is used to create a new process*/
    cpid=fork();

    if(cpid==0)
    {
        while(1)
        {
            bzero(&recvBuffer,sizeof(recvBuffer));

            /*Receiving the request from client*/
            count = recv(clientSocketDescriptor,recvBuffer,sizeof(recvBuffer),0);
            if(recvBuffer[0]=='E' && recvBuffer[1]=='N' && recvBuffer[2]=='D')
            {
                printf("\nClient disconnected.\n");
                fprintf(log,"%s    Client at port %i disconnected.\n\n",asctime(tm),5500);
                fclose(log);
                close (clientSocketDescriptor);
                kill(getpid()-1,SIGQUIT);
                kill(getpid(), SIGQUIT);
            }
            if (count != -1)
            {
                fprintf(log,"%s    CLIENT: %s\n",asctime(tm),recvBuffer);
                printf("\nCLIENT : %s",recvBuffer);
            }

        }
    } else
    {
        while(1)
        {

            bzero(&sendBuffer,sizeof(sendBuffer));
            printf("\nType a message here: ...");

            /*Read the message from client*/
            fflush(stdin);
            fgets(sendBuffer,10000,stdin);

            /*Sends the message to client*/
            fprintf(log,"%s    SERVER: %s\n",asctime(tm),sendBuffer);
            send(clientSocketDescriptor,sendBuffer,strlen(sendBuffer)+1,0);
            printf("\nMessage sent !");
        }
    }
return 0;
}
