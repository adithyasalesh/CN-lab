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

int socket_desc,client_sock,client_size,n;
struct sockaddr_in server_addr,client_addr;
FILE *fp;
char name[100],fname[100],file[100],fileread[100],ch,rcv[100];

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

n=recv(client_sock,rcv,100,0);
rcv[n]='\0';
fp=fopen(rcv,"r");
if(fp==NULL)
{
    send(client_sock,"error",5,0);
    close(client_sock);
}
else
{
    while(fgets(fileread,sizeof(fileread),fp))
    {
        if(send(client_sock,fileread,sizeof(fileread),0)<0)
        {
            printf("cant send file contents");
        }
        sleep(1);

    }
    if(!fgets(fileread,sizeof(fileread),fp))
    {
        printf("done");
        send(client_sock,"completed",9,0);
    }
}

close(socket_desc);
close(client_sock);
return 0;
}