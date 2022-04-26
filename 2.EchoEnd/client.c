#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLINE 1024
#define PORT 5035

int main(){
  // socket descriptor creation in udp mode
  int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);

  // for storing  address of address
  socklen_t addressLength;

  // storing address in serverAddress
  struct sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(PORT);

  // checking connection
  connect(socketDescriptor,(struct sockaddr*)&serverAddress,sizeof(serverAddress));

  char sendMessage[MAXLINE],recvMessage[MAXLINE];

  while (1)
  {
    printf("\nEnter message :");
    fgets(sendMessage,sizeof(sendMessage),stdin);

    // sending and receiving the messages
    send(socketDescriptor,sendMessage,MAXLINE,0);
    if(sendMessage[0]=='E' && sendMessage[1]=='N' && sendMessage[2]=='D')
    {
        printf("\nClientClosed\n");
        close(socketDescriptor);
        return 0;
    }
    recv(socketDescriptor,recvMessage,MAXLINE,0);
    printf("\nServer's Echo :%s\n",recvMessage);
  }

  return 0;
}
