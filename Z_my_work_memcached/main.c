/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: Thu 17 Nov 2016 03:24:22 PM CST
 ************************************************************************/


#include <stdio.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include"anet.h"
#include"ae.h"
#include"log.h"
#include"defines.h"
#include"tcp_server.h"

int init_socket()
{
    int fd = -1;

    fd = anetTcpServer(NULL,SERVER_PORT,NULL,500);
    if(fd < 0)
    {
        server_log(LOG_ERROR,"anetCreateSocket error and we quit\n");
        printf("anetCreateSocket error and we quit\n");
        exit(-1);
    }
    anetNonBlock(NULL,fd);

    return fd;
}
void daemonize()
{
    if(fork() != 0) exit(0);
    setsid();

    return;
}
void create_pid_file()
{
    FILE * file;
    file = fopen(PID_FILE_NAME,"w");
    if(file)
    {
        fprintf(file,"%d\n",(int)getpid());
        fclose(file);
    }
    return;
}
void shutdownNow(int arg)
{
    unlink(PID_FILE_NAME);
    exit(0);

    return;
}
ServerStat server_stat;
int main(int argc,char *argv[])
{
    aeEventLoop *Event = NULL;

    daemonize();
    init_log_level(LOG_NOTICE);
    create_pid_file();
    unlink(LOG_FILE_NAME);
    server_log(LOG_ALERT,"Server initializing... ...\n");

    signal(SIGPIPE,SIG_IGN);
    signal(SIGTERM,shutdownNow);
    signal(SIGKILL,shutdownNow);
    signal(SIGQUIT,shutdownNow);
    signal(SIGINT,shutdownNow);
    
    memset(&server_stat,0,sizeof(server_stat));
    init_tcp_server();
    Event  = aeCreateEventLoop(1000);

    server_stat.server_fd = init_socket();
    aeCreateFileEvent(Event,server_stat.server_fd,AE_READABLE,server_accept_handle,NULL);

    server_log(LOG_ALERT,"Server started now enjoy ~~~\n");
    aeMain(Event);




    return 0;
}
