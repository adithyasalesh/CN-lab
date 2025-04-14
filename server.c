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

int main(void)
{
int socket_desc,client_sock,client_size;
struct sockaddr_in server_addr,client_addr;
char clientmessage[2000],servermessage[2000];

memset(servermessage,'\0',sizeof(servermessage));
memset(clientmessage,'\0',sizeof(clientmessage));

#ifdef _WIN32
WSADATA wsa;
if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
    printf("Failed to initialize Winsock. Error Code : %d", WSAGetLastError());
    return 1;
}
#endif



socket_desc = socket(AF_INET, SOCK_STREAM, 0);
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

if(listen(socket_desc,1)<0)
{
    printf("failed to listen");

}
else
{
printf("listening for connectiions..........\n");
}
client_size=sizeof(client_addr);
client_sock=accept(socket_desc,(struct sockaddr*)&client_addr,&client_size);
if(client_sock<0)
{
    printf("failed to connect");
    return -1;
}
else
{
    printf("connected......\n");

}
printf("client connected at %s and port %i.......\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
while(1)
{
    memset(clientmessage, '\0', sizeof(clientmessage));
if(recv(client_sock,clientmessage,sizeof(clientmessage),0)<0)
{
    printf("failed to recieve");
    return -1;
}
else
{
    printf ("this is clients message %s\n",clientmessage);

}
strcpy(servermessage,"this is the server message");
if(send(client_sock,servermessage,strlen(servermessage),0)<0)
{
    printf("failed to send");
    return -1;
}

}
close(client_sock);
close(socket_desc);
return 0;

}