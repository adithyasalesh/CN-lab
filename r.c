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
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
struct timeval timeout;
void func(int connfd)
{
    char buff[MAX];
    int f, c, ack, next = 0;
    while (1)
    {
        sleep(1);
        bzero(buff, MAX);
        recv(connfd, buff, MAX, 0);
        if (strcmp("Exit", buff) == 0)
        {
            printf("Exit\n");
            break;
        }
        f = atoi(buff);
        if (f != next)
        {
            printf("Frame %d discarded\nAcknowledgement sent: %d\n", f, ack);
            bzero(buff, MAX);
            snprintf(buff, sizeof(buff), "%d", ack);
            send(connfd, buff, sizeof(buff), 0);
            continue;
        }
        c = rand() % 3;
        switch (c)
        {
            case 0:
                // printf("Frame %d not received\n",f);
                break;
            case 1:
                ack = f;
                sleep(2);
                printf("Frame %d received\nAcknowledement sent: %d\n", f, ack);
                bzero(buff, MAX);
                snprintf(buff, sizeof(buff), "%d", ack);
                send(connfd, buff, sizeof(buff), 0);
                next = ack + 1;
                break;
            case 2:
                ack = f;
                printf("Frame %d received\nAcknowledement sent: %d\n", f, ack);
                bzero(buff, MAX);
                snprintf(buff, sizeof(buff), "%d", ack);
                send(connfd, buff, sizeof(buff), 0);
                next = ack + 1;
                break;
        }
    }
}
void main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
    #ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error Code : %d", WSAGetLastError());
        return 1;
    }
    #endif
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Socket creation failed\n");
        exit(0);
    }
    else
        printf("Socket successfully created\n");
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("socket bind failed\n");
        exit(0);
    }
    else
        printf("Socket successfully binded\n");
    if ((listen(sockfd, 5)) != 0)
    {
        printf("Listen failed\n");
        exit(0);
    }
    else
        printf("Server listening\n");
    len = sizeof(cli);
    connfd = accept(sockfd, (SA *)&cli, &len);
    if (connfd < 0)
    {
        printf("Server accept failed\n");
        exit(0);
    }
    else
        printf("Server accept the client\n");
    func(connfd);
    close(sockfd);
}