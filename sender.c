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
int main(void)
{
int socket_desc;
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

if(connect(socket_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
{
    printf("failed to connect");
    return -1;
}
else
{
    printf("connection succesfull\n");

}
while(k!=0)
{
    if(m<=5)
    {
        printf("sending frame %d\n",m);
    }
    //we are forcfully making odd frames lost;
    if(m%2==0)
    {
        strcpy(buffer,"frame");

    }
    else
    {
        strcpy(buffer,"error");
        printf("packet loss\n");
        for(p=0;p<3;p++)
        {
            printf("waiting for %d seconds\n",p);
            sleep(1);
        }
        printf("retransmiting lost packet....\n");
        strcpy(buffer,"frame");
        sleep(3);
    }
    //sending frame to reciever;
    int y=send(socket_desc,buffer,strlen(buffer),0);
    if(y==-1)
    {
        printf("failed to send");
    }
    else 
    {
        printf("sent frame %d\n",m);
    }
    //recieving acknowedgemnt
    int z=recv(socket_desc,buffer,1024,0);
    if(z==-1)
    {
        printf("failed to recieve data");
    }
    if(strncmp(buffer,"ack",3)==0)
    {
        printf("recieved ack for frame %d\n",m);
    }
    else if (strncmp(buffer,"kca",3)==0)
    {
        printf(" ack  lost for frame %d\n",m);
    }
    k--;
    m++;

}

close (socket_desc);
return 0;
}