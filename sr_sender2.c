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
void func(int socket_desc,int nf,int ws)
{
char buff[MAX];
int ack,i=0,n,k,w1=0,w2=ws-1,j,flag=0,count_ack=0;
if(setsockopt(socket_desc,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout))<0)
{
    perror("setsockopt(SO_RCVTIMEO)FAILED");
}
for(i=0;i<nf && i<=w2;i++)
{
    bzero(buff,sizeof(buff));
    snprintf(buff,sizeof(buff),"%d",i);
    k=send(socket_desc,buff,sizeof(buff),0);
    printf("frame %d sent\n",i);
}

while(1)
{
    if(count_ack==nf)
    {
        strcpy(buff,"Exit");
        k=send(socket_desc,buff,sizeof(buff),0);
        break;
    }

    if(w2-w1!=ws-1 && flag==0 && i!=nf)
    {
    
        bzero(buff,sizeof(buff));
        snprintf(buff,sizeof(buff),"%d",i);
        k=send(socket_desc,buff,sizeof(buff),0);
        printf("frame %d sent\n",i);
        w2++;
        i++;
    }
    flag=0;
    bzero(buff,sizeof(buff));
    n=recv(socket_desc,buff,MAX,0);
    ack=atoi(buff);
    
    if(n>0)
    {
        if(ack+1 == nf)
        {
            printf("Ack recieved for frame %d\n",ack);
            bzero(buff,sizeof(buff));
            count_ack++;
           /* strcpy(buff,"exit");
            k=send(socket_desc,buff,sizeof(buff),0);
            break;*/
        }
        else if (ack==-1)
        {   
            printf("ack not recieved .resending frame %d\n",w1);
            bzero(buff,sizeof(buff));
            snprintf(buff,sizeof(buff),"%d",w1);
            k=send(socket_desc,buff,sizeof(buff),0);
            printf("frame %d sent\n",w1);
        }
        else
        {
            w1++;
            printf("Ack recived for frame %d\n",ack);
            count_ack++;
        }
    }
}
}

int main(void)
{
    int socket_desc,f,w;
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
server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

timeout.tv_sec=3;
timeout.tv_usec=0;

if(connect(socket_desc,( SA *)&server_addr,sizeof(server_addr))<0)
{
    printf("failed to connect");
    return -1;
}
else
{
    printf("connection succesfull\n");

}

printf("Enter the number of frames:");
scanf("%d",&f);
printf("Enter the window size:");
scanf("%d",&w);
func(socket_desc,f,w);
close(socket_desc);
}

