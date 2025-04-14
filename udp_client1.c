#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#ifdef _WIN32
#include<winsock2.h>
#else
#include<sys/socket.h>
#include<arpa/inet.h>
#endif

#ifndef _WIN32
#include <sys/types.h>
#else
typedef int socklen_t; // Define socklen_t for Windows
#endif

int main(void)
{
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed to initialize Winsock.\n");
        return -1;
    }
#endif

int sock_desc;
struct sockaddr_in server_addr,client_addr;
char clientmessage[2000],servermessage[2000];
socklen_t server_struct_length=sizeof(server_addr);

memset(servermessage,'\0',sizeof(servermessage));
memset(clientmessage,'\0',sizeof(clientmessage));

sock_desc=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
if(sock_desc<0)
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

printf("enter message:\n");
gets(&clientmessage);

if(sendto(sock_desc,clientmessage,strlen(clientmessage),0,(struct sockaddr*)&server_addr,server_struct_length)<0)
{
    printf("failed to send ");
    return -1;
}
if(recvfrom(sock_desc, servermessage, sizeof(servermessage), 0, (struct sockaddr*)&server_addr,&server_struct_length) < 0)
{
    printf("failed");
    return -1;
}
else
{
    printf("%s",servermessage);
}
close(sock_desc);
return 0;
}