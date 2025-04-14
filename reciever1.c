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
#ifndef _WIN32
#include<netdb.h>
#include<netinet/in.h>
#endif
#include<sys/types.h>
#include<sys/time.h>
#include<strings.h>
# define MAX 80
# define PORT 8080
# define SA struct sockaddr
struct timeval timeout;
void func(int client_sock)
{
    char buff[MAX];
    int f,c,ack,next=0;
    while(1)
    {
       sleep(1);
       bzero(buff,MAX);
       recv(client_sock,buff,MAX,0);
       if(strcmp("exit" ,buff)==0)
       {
          printf("exit");
          break;
       }
       f=atoi(buff);
       if(f!=next)
       {
        printf("frame %d discarded Ack sent :%d\n",f,ack);
        bzero(buff,sizeof(buff));
        snprintf(buff,sizeof(buff),"%d",ack);
        send(client_sock,buff,sizeof(buff),0);
        continue;
       }
       c=rand()%3;
       switch(c)
       {
        case 0:
        {     
            
            break;
        }
        case 1:
        {
            ack=f;
            sleep(2);
            printf("frame %d recieved, Ack sent :%d\n",f,ack);
            bzero(buff,sizeof(buff));
            snprintf(buff,sizeof(buff),"%d",ack);
            send(client_sock,buff,sizeof(buff),0);
            next=ack+1;
            break;
        }
        case 2:

        {
            ack=f;
            printf("frame %d recieved, Ack sent :%d\n",f,ack);
            bzero(buff,sizeof(buff));
            snprintf(buff,sizeof(buff),"%d",ack);
            send(client_sock,buff,sizeof(buff),0);
            next=ack+1;
            break;
        }
       }
    }
}
int main(void)
{
    int socket_desc,client_sock,client_size;
    struct sockaddr_in server_addr,client_addr;

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

bzero(&server_addr,sizeof(server_addr));

server_addr.sin_family=AF_INET;
server_addr.sin_port=htons(PORT);
server_addr.sin_addr.s_addr=htonl(INADDR_ANY);

if(bind(socket_desc,( SA *)&server_addr,sizeof(server_addr))<0)
{
    printf("failed to bind");
    return -1;
}
else
{
    printf("binding succesfull\n");

}
if(listen(socket_desc,5)!=0)
{
    printf("failed to listen\n");
    return -1;

}
else{
    printf("listening for incoming connections........\n");
}

 client_size=sizeof(client_addr);
 client_sock=accept(socket_desc,(SA *)&client_addr,&client_size);
 if(client_sock<0)
 {
    printf("failed to connect");
    return -1;
 }
 else
 {
    printf("sender connected at ip %s and port %i",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
 }
 func(client_sock);
 close(socket_desc);

}