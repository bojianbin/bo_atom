/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: Sunday, March 10, 2019 AM04:12:52 CST
 ************************************************************************/

#include<stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "anet.h"
#include <signal.h>
#include "ae.h"



int nettool_test(int argc,char *argv[])
{
    int fd ; 


    fd  = anetTcpServer(NULL,atoi(argv[1]),"0.0.0.0",50);
    if(fd < 0 )
    {
        printf("anetTcpServer error\n");
        exit(-1);
    }

    int clientfd;

    while( (clientfd = anetTcpAccept(NULL,fd,NULL,0,NULL)) > 0)
    {
        int ret ;
        char buf[500] = {0};

        memset(buf,0,500);
        while(read(clientfd,buf,520) > 0)
        {
            printf("%s",buf);
            memset(buf,0,500);
        }
        close(clientfd);
    }   

    return 0;
}
void SignalProc(struct aeEventLoop *eventLoop, int sig, void *clientData)
{
    printf("Get one\n");
}
int signal_test()
{
    aeEventLoop * loop = aeCreateEventLoop(20);
    aeSetSignalEventLoop(loop);
    aeCreateSignalEvent(loop,SIGINT,SignalProc,NULL);
    
    aeMain(loop);
}

int main()
{
    signal_test();

    return 0;
}