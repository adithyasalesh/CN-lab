#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#ifdef _WIN32
#include<winsock2.h>
#else
#include<arpa/inet.h>
#endif
#ifndef _WIN32
#include<sys/socket.h>
#endif
#include<string.h>
int main(void)
{
int socket_desc,client_sock,client_size;
struct sockaddr_in server_addr,client_addr;
char servermessage[2000],clientmessage[2000];
int k=5,m=1,p;
char buffer[1024];

#ifdef _WIN32
WSADATA wsa;
if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
    printf("Failed to initialize Winsock. Error Code : %d", WSAGetLastError());
    return 1;
}
#endif


socket_desc=socket(AF_INET,SOCK_STREAM,0);
if(socket_desc<0)
{
    printf("failed to create socket");
    return -1;
}
else{
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
    printf("binding succesfull\n");

}
if(listen(socket_desc,1)<0)
{
    printf("failed to listen\n");
    return -1;

}
else{
    printf("listening for incoming connections........\n");
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
    printf("sender connected at ip %s and port %i",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
 }
while(k!=0)
{
int y=recv(client_sock,buffer,1024,0);
if(y==-1)
{
    printf("failed to recieve data");
}
if(strncmp(buffer,"frame",5)==0)
    {
        printf("recieved  frame %d\n",m);
    }
    else if (strncmp(buffer,"error",5)==0)
    {
        printf(" frame  %d not recieved \n",m);
    }
if(m%2==0)
{
    strcpy(buffer,"ack");

}
else
{
    strcpy(buffer,"kca");
    printf("ack lost for frame %d",m);
    for(p=0;p<3;p++)
        {
            printf("waiting for %d seconds\n",p);
            sleep(1);
        }
        printf("retransmiting ack....\n");
        strcpy(buffer,"ack");
        sleep(3);
    
}
printf("sending ack %d\n",m);
int z=send(client_sock,buffer,strlen(buffer),0);
if(z==1)
{
    printf("failed to send");
    exit(0);
}
k--;
m++;
}

 close(socket_desc);
 close(client_sock);

return 0;
}