#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

#define MAXLINE 1024
#define PORT 5035

int main(){

  struct sockaddr_in  serverAddress,clientAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr=INADDR_ANY;
  serverAddress.sin_port=htons(PORT);

  int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);

  int number, come;
  socklen_t addressLength;
  char message[MAXLINE];

  bind(socketDescriptor,(struct sockaddr*)&serverAddress,sizeof(serverAddress));
  printf("\nServer Started ...\n");
  listen(socketDescriptor,5);

  while(1)
  {
    int clientLen = sizeof(clientAddress);
    come = accept(socketDescriptor,(struct sockaddr *) &clientAddress, &clientLen);
    break;
  }
  while(1)
  {
    number = recv(come,message,MAXLINE,0);
    if (message[0]=='E' && message[1]=='N' && message[2]=='D')
    {
        printf("\nServer's down\n");
        close(come);
        return 0;
    }
    printf("\n Client's Message:%s ",message);
    send(come,message,number,0);
  }
  close(come);
  return 0;
}
