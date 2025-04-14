#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#ifdef _WIN32
#include<winsock2.h>
#else
#include<sys/socket.h>
#endif
#ifdef _WIN32
#include<winsock2.h>
#else
#include<arpa/inet.h>
#endif
#include<string.h>

#ifdef _WIN32
typedef int socklen_t;
#endif

int main(void)
{
int socket_desc;
struct sockaddr_in server_addr,client_addr;
char clientmessage[2000],servermessage[2000];
socklen_t client_struct_length=sizeof(client_addr);

memset(servermessage,'\0',sizeof(servermessage));
memset(clientmessage,'\0',sizeof(clientmessage));

#ifdef _WIN32
WSADATA wsa;
if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
    printf("Failed to initialize Winsock. Error Code : %d", WSAGetLastError());
    return 1;
}
#endif



socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
if(socket_desc<0)
{
printf("failed to create socket");
return -1;
}
else
{
    printf("socket created\n");

}
server_addr.sin_family=AF_INET;
server_addr.sin_port=htons(2000);
server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

if(bind(socket_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
{
    printf("failed to bind");
    return -1;
}
else
{
    printf("binded succesfully\n");
}
if(recvfrom(socket_desc,clientmessage,sizeof(clientmessage),0,(struct sockaddr*)&client_addr,&client_struct_length)<0)
{
    printf("failed to recieve");
    return -1;
}
else
{
    printf("THIS IS THE CLIENTS MESSAGE %s",clientmessage);
}
strcpy(servermessage,"servers response");
if(sendto(socket_desc,servermessage,strlen(servermessage),0,(struct sockaddr*)&client_addr,client_struct_length)<0)
{
    printf("failed");
    return -1;

}
else
{
    printf("sended message succesfully");
}
close (socket_desc);
return 0;
}
