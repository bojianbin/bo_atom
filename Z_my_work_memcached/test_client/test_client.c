/*************************************************************************
    > File Name: test_client.c
    > Author: 
    > Mail: 
    > Created Time: Fri 18 Nov 2016 03:12:17 PM CST
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
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include "anet.h"
#include "defines.h"
int Client_socket = -1;
int start = 0;
int _bits(int data)
{
    int ret = 1;

    while((data = data/10)) ret++;
    
    return ret ;
}
void* send_data(void *arg)
{
    int i = 0;
    char buf[1024] = {0};
    int ret ;

    srand((unsigned)time(NULL));
    while(1)
    {
    start = rand()%10000;
    for(i = start; i < start + 20 ; i++)
    {
        memset(buf,0,1024);
        sprintf(buf,"*3\r\n$3\r\nset\r\n$%d\r\nname%d\r\n$%d\r\nbojianbin%d\r\n",\
                (_bits(i) + 4),i,(_bits(i) + 9),i);
        ret = write(Client_socket,buf,strlen(buf));
        usleep(10000);
        if(ret < 0)
        {
            printf("server error1:%s\n",strerror(errno));
            return NULL;
        }
    }
    for(i = start; i < start + 20 ; i++)
    {
        memset(buf,0,1024);
        sprintf(buf,"*2\r\n$3\r\nget\r\n$%d\r\nname%d\r\n",\
                (_bits(i) + 4),i);
        ret = write(Client_socket,buf,strlen(buf));
        if(ret < 0)
        {
            printf("server error2:%s\n",strerror(errno));
            return NULL;
        }
    }
    usleep(100000);
    }

    return NULL;
}
void _my_print(char *buf,int len)
{
    char *ptr = NULL;

    for(ptr = buf;ptr - buf < len; ptr++)
    {
        ptr = strstr(ptr,"\r\n");
        if(ptr)
        {
            *ptr = ' ';
            *(ptr+1) = ' ';
            ptr += 2;
        }else
        {
            break;
        }
    }
    
    for(ptr = buf;ptr - buf < len; ptr++)
    {
        ptr = strstr(ptr,"*");
        if(ptr)
        {
            *ptr = '\n';
            ptr += 1;
        }else
        {
            return;
        }
    }
}
#define READ_BUF_SIZE 1024
void * read_data(void *arg)
{
    char buf[READ_BUF_SIZE] = {0};
    int ret ;

    while((ret = read(Client_socket,buf,READ_BUF_SIZE - 1)) >= 0)
    {
        _my_print(buf,ret);
        buf[ret] = '\0';
        printf("%s",buf);
    }
    printf("exit read_data %s  %d\n",strerror(errno),ret );
    return NULL;
}
void simple()
{
    char buf[1024] = {0};
    char *cmd1 = "*3\r\n$3\r\nset\r\n$4\r\nname\r\n$9\r\nbojianbin\r\n";
    char *cmd2 = "*2\r\n$3\r\nget\r\n$4\r\nname\r\n";

    write(Client_socket,cmd1,strlen(cmd1));
    write(Client_socket,cmd2,strlen(cmd2));
    sleep(2);
    read(Client_socket,buf,1024);
    printf("%s\n",buf);

    return;
}
int main(int argc,char *argv[])
{
    int i;
    pthread_t tid;
    signal(SIGPIPE,SIG_IGN);

    Client_socket = anetTcpConnect(NULL,"127.0.0.1",SERVER_PORT);
    
    pthread_create(&tid,NULL,send_data,NULL);
    pthread_create(&tid,NULL,read_data,NULL);
    //simple();
    while(1)
    {
        sleep(10);
    }
    close(Client_socket);
    
    return 0;
}
