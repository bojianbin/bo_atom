/*************************************************************************
	> File Name: tcp_server.h
	> Author: 
	> Mail: 
	> Created Time: Fri 18 Nov 2016 02:41:38 PM CST
 ************************************************************************/

#ifndef _TCP_SERVER_H
#define _TCP_SERVER_H

#include<stdint.h>

#include"ae.h"
#include"defines.h"

#define ADDSEQ(client) (\
    {\
        if(client) \
        {\
            client->gSeq++;\
            if(client->gSeq == 0)\
            {\
                client->gSeq = 0x8000;\
            }\
        }\
    }\
    )
typedef struct 
{
    int server_fd;
    int client_fd;
}ServerStat;

typedef struct ClientStat_s
{
    int fd;
    char ip[64];
    int port;

    long long time_event_id[5];
    uint32_t noHeartbeatPeriod;//In seconds
    uint16_t gSeq;

    uint8_t recv_cmd[CLIENT_RECV_CMD_LENGTH];
    int recv_pos;

    uint8_t send_response[CLIENT_SEND_RESPONSE_LENGTH];
    int send_pos;

    struct ClientStat_s * next;
    struct ClientStat_s * pre;

}ClientStat;


int init_tcp_client(aeEventLoop *);

void server_accept_handle(struct aeEventLoop *eventloop,int fd,void *clientData,int mask);

int  heart_beat_handle(struct aeEventLoop *eventloop,long long id ,void *arg);

void recv_cmd_handle(struct aeEventLoop *eventloop,int fd,void *clientData,int mask);

int  check_triger_handle(struct aeEventLoop *eventloop,long long id ,void *arg);
#endif
