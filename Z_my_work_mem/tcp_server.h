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
typedef struct 
{
    int server_fd;

}ServerStat;

typedef struct Cmd_s
{
    int args_num;

    char *value[CMD_ARGS_MAX];
    uint32_t item_length[CMD_ARGS_MAX];
    
    uint32_t item_pos[CMD_ARGS_MAX];
    int cur_item;

    struct Cmd_s *next;
}Cmd;
typedef struct Response_s 
{
    int args_num;

    char *value[CMD_ARGS_MAX];
    uint32_t item_length[CMD_ARGS_MAX];
    uint32_t item_pos[CMD_ARGS_MAX];

    int cur_item;


    struct Response_s *next;
}Response;

typedef struct ClientStat_s
{
    int fd;
    char ip[64];
    int port;

    char recv_cmd[CLIENT_RECV_CMD_LENGTH];
    Cmd *cur_cmd_item;
    Cmd *recv_cmd_queue_head; 
    Cmd *recv_cmd_queue_tail; 
    int recv_pos;

    char send_response[CLIENT_SEND_RESPONSE_LENGTH];
    Response *send_response_queue_head ;
    Response *send_response_queue_tail ;
    int send_pos;

    struct ClientStat_s * next;
    struct ClientStat_s * pre;
}ClientStat;

typedef struct
{
    void * item;
    uint32_t length; 
}DictItem;

#define ResponseInsert(client,res) ( \
        {\
        if(res)\
        {\
        if(client->send_response_queue_head == NULL && client->send_response_queue_tail == NULL)\
        { client->send_response_queue_head = client->send_response_queue_tail = res;\
          res->next = NULL;\
        }else\
        {\
        client->send_response_queue_tail->next = res;res->next = NULL;\
        client->send_response_queue_tail = res;\
        }\
        }\
        }\
        )
#define ResponseHead(client) (client->send_response_queue_head)
#define ResponseDeleteHead(client)(\
        {\
        if(client->send_response_queue_head != NULL)\
        {\
            Response * tmp;tmp = client->send_response_queue_head->next;\
            free(client->send_response_queue_head);\
            if(client->send_response_queue_head == client->send_response_queue_tail) client->send_response_queue_tail = NULL;\
            client->send_response_queue_head = tmp;\
        }\
        }\
        )
static inline int ResponseCount(ClientStat *client)
{
    int count = 0;
    Response * tmp = NULL;

    if(!client) return 0;
    for(tmp = client->send_response_queue_head;tmp;tmp = tmp->next)
    {
        count++;
    }

    return count;
}


#define cmd(client) (client->cur_cmd_item)

#define CmdInsert(client,cmd) ( \
        {\
        if(client->recv_cmd_queue_head == NULL && client->recv_cmd_queue_tail == NULL)\
        { client->recv_cmd_queue_head = client->recv_cmd_queue_tail = cmd;\
          cmd->next = NULL;\
        }else\
        {\
        client->recv_cmd_queue_tail->next = cmd;cmd->next = NULL;\
        }\
        }\
        )

#define CmdDeleteHead(client)(\
        {\
        if(client->recv_cmd_queue_head != NULL)\
        {\
            Cmd * tmp;tmp = client->recv_cmd_queue_head->next;\
            free(client->recv_cmd_queue_head);\
            if(client->recv_cmd_queue_head == client->recv_cmd_queue_tail) client->recv_cmd_queue_tail = NULL;\
            client->recv_cmd_queue_head = tmp;\
        }\
        }\
        )

#define CmdHead(client) (client->recv_cmd_queue_head)
static inline int CmdCount(ClientStat *client)
{
    int count = 0;
    Cmd * tmp = NULL;

    if(!client) return 0;
    for(tmp = client->recv_cmd_queue_head;tmp;tmp = tmp->next)
    {
        count++;
    }

    return count;
}
static inline int CmdComplete(ClientStat * client,Cmd * cmd)
{
    int num;

    if(!client || !cmd) return 0;
    num = cmd->args_num;
    if(!num) return 0;
    if(cmd->item_pos[num - 1] >= cmd->item_length[num - 1])
        return 1;
    return 0;
}

int init_tcp_server();
void server_accept_handle(struct aeEventLoop *eventloop,int fd,void *clientData,int mask);

#endif
