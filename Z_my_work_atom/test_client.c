/*************************************************************************
	> File Name: test-client.c
	> Author: 
	> Mail: 
	> Created Time: 2017年06月16日 星期五 14时58分27秒
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
#include <pthread.h>

#pragma pack(1)

#include "anet.h"
#define SERVER_PORT 1502
    
int Client_socket = 0 ;
int clientfd = 0;
char client_address [50] = {0};
int client_port = 0;


typedef struct 
{
    //0x5a
    uint8_t start;
    //CMD_ASK CMD_ANSER CMD_INFORM CMD_ACK CMD_HEARTBEAT
    uint8_t cmd;
    uint8_t id[6];
    uint16_t seq;
    uint16_t len;
}LotHead;

typedef struct
{
    LotHead head;
    uint8_t *data;
}LotMsg;

typedef struct 
{
    uint16_t addr;
    uint8_t tag;
    uint8_t register_addr;
}LotDataAtom;

static int index_ = 0;
void send_cmd(int fd)
{
    int i = 0;

    uint8_t buf[1024] = {0};
    LotMsg * lot = (LotMsg *)buf;
    LotDataAtom * data = (LotDataAtom *)(&(lot->data));

    lot->head.start = 0x5a;
    lot->head.cmd = 0x10;
    lot->head.len = htons(sizeof(LotDataAtom) + 6);

    data->addr = htons(0x0100);
    data->tag = 0x01;
    data->register_addr = 0x02;
    
    printf("In send cmd:");
    for(i =0 ; i < 6 ; i++)
    {
        if(i == index_)
            *(&(data->register_addr) + i + 1) = 0x01;
        else
            *(&(data->register_addr) + i + 1) = 0x00;

        printf("%d ",*(&(data->register_addr) + i + 1));
        
    }
    printf("\n");
    index_++;
    if(index_ >= 6)index_ = 0;

    memmove(buf + lot->head.len + sizeof(LotHead),buf,ntohs(lot->head.len) + sizeof(LotHead));


    write(fd,buf , (ntohs(lot->head.len) + sizeof(LotHead))  );

    return;
    
}
int readn(int fd,uint8_t * buf,int size)
{
    int sum = 0;
    int ret = 0;

    while(1)
    {
        ret  = read(fd,buf + sum ,size - sum);
        if(ret > 0)
        {
            sum += ret;
            if(sum >= size)
            {
                return 0;
            }
        }else
        {
            return -1;
        }
    }

}
void * recv_trigger(void * arg)
{
    uint8_t buf[1024] = {0};
    int i = 0;

    LotMsg * lot = (LotMsg *)buf;
    LotDataAtom * data = (LotDataAtom *)(&(lot->data));
   

    while(1)
    {
        memset(buf,0,1024);
        i = readn(clientfd,buf,sizeof(LotHead));
        if(i < 0)
        {
            perror("client disconnect:");
            exit(-1);
        }
        if(ntohs(((LotHead *)buf)->len) != 0)
            readn(clientfd,buf + sizeof(LotHead),ntohs(((LotHead *)buf)->len));

        for( i = 0 ; i < ntohs(((LotHead *)buf)->len) + sizeof(LotHead) ;i++)
        {
            printf("%x ",buf[i]);
        }
        printf("\n");

        if( ((LotHead *)buf)->cmd == 0x80 )
        {
            write(clientfd,buf,sizeof(LotHead));
        }
        if( ((LotHead *)buf)->cmd == 0x30 )
        {
            send_cmd(clientfd);
        }
    }
    return NULL;

}
int main()
{
    pthread_t tid;

    signal(SIGPIPE,SIG_IGN);

    Client_socket = anetTcpServer(NULL,SERVER_PORT,NULL,0);
    
    clientfd = anetTcpAccept(NULL,Client_socket,client_address,50,&client_port);
    
    pthread_create(&tid,NULL,recv_trigger,NULL);
   
    while(1)
    {
        
        sleep(5);
    }
    return 0;
}
