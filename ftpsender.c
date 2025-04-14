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
int n,s;
FILE *fp;
char name[100],fname[100],file[100],fileread[100],ch,rcvg[100];

struct sockaddr_in server_addr,client_addr;


sock_desc=socket(AF_INET,SOCK_STREAM,0);
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

if(connect(sock_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
{
    printf("failed to connect");
    return -1;
}
else
{
    printf("connected succesfully\n");
}
printf("enter filename:");
scanf("%s",name);
printf("enter newfilename:");
scanf("%s",fname);
fp=fopen(fname,"w");
send(sock_desc,name,sizeof(name),0);
while(1)
{
    s=recv(sock_desc,rcvg,sizeof(rcvg),0);
    rcvg[s]='\0';
    if(strcmp(rcvg,"error")==0)
    {
        printf("file not available ");
        exit(1);
    }
    if (strcmp(rcvg,"completed")==0)
    {
        printf("file is transferred.\n");
        
        fclose(fp);
        break;
    }
    else{
        fputs(rcvg,stdout);
        fprintf(fp,"%s",rcvg);
    }


}

close(sock_desc);
}