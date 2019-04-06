/*************************************************************************
	> File Name: tcp_server.c
	> Author: 
	> Mail: 
	> Created Time: Fri 18 Nov 2016 02:41:14 PM CST
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
#include <errno.h>

#include "tcp_server.h"
#include "ae.h"
#include "anet.h"
#include "log.h"
#include "cmd.h"


ClientStat *ClientQueue = NULL;

void clientAdd(ClientStat * tmp)
{
    if(!tmp) return;
    tmp->next = ClientQueue;
    if(ClientQueue) ClientQueue->pre = tmp;
    ClientQueue = tmp;
    ClientQueue->pre = NULL;

    return;
}

void clientDelete(ClientStat *tmp,struct aeEventLoop *eventloop)
{
    if(!tmp)  return;
    
    if(tmp == ClientQueue)
    {
        ClientQueue = tmp->next;
        if(ClientQueue) ClientQueue->pre = NULL;
    }else
    {
        tmp->pre->next = tmp->next;
        if(tmp->next) tmp->next->pre = tmp->pre;
    }  
    
    aeDeleteFileEvent(eventloop,tmp->fd,AE_READABLE|AE_WRITABLE);
    close(tmp->fd);
    
    {
        int i = 0;
        for(i = 0;i < 5 ; i++)
        {
            if(tmp->time_event_id[i] >= 0)
            {
                aeDeleteTimeEvent(eventloop,tmp->time_event_id[i]);
            }
        }
    }
    free(tmp);
   
    return;
}

void recv_cmd_handle(struct aeEventLoop *eventloop,int fd,void *clientData,int mask);
void send_response_handle(struct aeEventLoop *eventloop,int fd,void *clientData,int mask);

void server_accept_handle(struct aeEventLoop *eventloop,int fd,void *clientData,int mask)
{
    int ret ;
    int clientfd = -1;
    char client_address [50] = {0};
    int client_port = 0;
    ClientStat * client_stat = NULL;

    while( (clientfd = anetTcpAccept(NULL,fd,client_address,50,&client_port)) > 0)
    {
        server_log(LOG_NOTICE,"get connect from %s %d\n",client_address,client_port);

        ret = anetNonBlock(NULL,clientfd);
        if(ret < 0)
        {
            server_log(LOG_ALERT,"anetNonBlock error\n");
            return;
        }

        client_stat = (ClientStat *)malloc(sizeof(*client_stat));
        memset(client_stat,0,sizeof(*client_stat));
        client_stat->fd = clientfd;
        client_stat->port = client_port;
        strncpy(client_stat->ip,client_address,64);
        clientAdd(client_stat); 
        aeCreateFileEvent(eventloop,client_stat->fd,AE_READABLE,recv_cmd_handle,client_stat);
    }
    return ;
}


void recv_cmd_handle(struct aeEventLoop *eventloop,int fd,void *clientData,int mask)
{
    int ret ;
    ClientStat *client = (ClientStat *)clientData;
   
    while(1)
    {
        errno = 0;
       
        //recv buffer full . delete this .we do not allow this
        if(CLIENT_RECV_CMD_LENGTH - client->recv_pos <= 0)
        {
            server_log(LOG_NOTICE,"disconnect:recv buffer full:%s : %d\n",client->ip,client->port);
           clientDelete(client,eventloop);
           return;
        }


        ret = read(client->fd,client->recv_cmd + client->recv_pos,CLIENT_RECV_CMD_LENGTH - client->recv_pos);
        if(ret > 0)
        {
            client->recv_pos += ret ;
            while(1)//something
            {
                
                if( 1)//something
                {
                    uint32_t ret ;
                    ret = do_protocol(client);
                    
                    //do one step
                    //memmove(client->recv_cmd,........);
                    //client->recv_pos -= ;
                    if(ret & SEND_CMD )
                    {
                        aeCreateFileEvent(eventloop,client->fd,AE_WRITABLE,send_response_handle,client);
                    }
                    if(ret & RECV_CMD)
                    {
                        aeCreateFileEvent(eventloop,client->fd,AE_READABLE,recv_cmd_handle,client);
                    }
                    if(ret & ERR_SENDBUFFERFULL_CMD)
                    {
                        server_log(LOG_NOTICE,"disconnect:send buffer full:%s : %d\n",client->ip,client->port);
                        clientDelete(client,eventloop);
                        return ;
                    }
                }else{
                    break;
                }
            }
        }else if(ret == 0)
        {
            server_log(LOG_NOTICE,"disconnect:read==0 :%s : %d\n",client->ip,client->port);
            clientDelete(client,eventloop);
            return;

        }else
        {
            if(errno == EWOULDBLOCK || errno == EAGAIN)
            {
                return ;
            }
            server_log(LOG_NOTICE,"disconnect:read error :%s : %d\n",client->ip,client->port);
            clientDelete(client,eventloop);
            return ;
        }
    }


    return ;
}
void send_response_handle(struct aeEventLoop *eventloop,int fd,void *clientData,int mask)
{
    int cur_item = 0;
    int ret = 0;
    ClientStat *client = (ClientStat *)clientData;
   
    while(client->send_pos)
    {

        ret = write(client->fd,client->send_response,client->send_pos);
        if(ret <= 0)
        {
            if(errno == EWOULDBLOCK || errno == EAGAIN)
            {
                return;
            }
            server_log(LOG_NOTICE,"disconnect:write error :%s : %d\n",client->ip,client->port);
            clientDelete(client,eventloop);
            return ;
        }else
        {
            memmove(client->send_response,client->send_response + ret,client->send_pos - ret);
            client->send_pos -= ret;
        }
    }

    aeDeleteFileEvent(eventloop,fd,AE_WRITABLE);


    return;
}

