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
void _clientFreebuf(ClientStat *client)
{
    int pos;

    if(!client)  return ;
    while(CmdHead(client))
    {
        for(pos = 0;pos < CmdHead(client)->args_num ; pos++)
        {
            if(CmdHead(client)->value[pos] != NULL)
            {
                free(CmdHead(client)->value[pos]);
                CmdHead(client)->value[pos] = NULL;
            }
        }
        CmdDeleteHead(client);
    }
    while(ResponseHead(client))
    {
        ResponseDeleteHead(client);
    }

    return ;
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
    _clientFreebuf(tmp);
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
        aeCreateFileEvent(eventloop,client_stat->fd,AE_WRITABLE,send_response_handle,client_stat);
    }
    return ;
}


int _check_cmd_complete2(aeEventLoop *eventloop,ClientStat *_stat,int total_len)
{
    char *head  = _stat->recv_cmd;
    char * pre  = head;
    char * end  = pre;
    
    if(_stat == NULL)
        return -1;
   
    while(1)
    {
        int num; 
        
        if(!cmd(_stat) || cmd(_stat)->cur_item  >= cmd(_stat)->args_num)
        {
            Cmd * tmp_Cmd;
            while(pre < head + total_len && *pre != '*')
            {
                pre++;
                if(pre >= head + total_len)
                {
                    _stat->recv_pos = 0;
                    return 0;
                }
            }
            end = strstr(pre,"\r\n");
            if(end == NULL)
            {
                if(_stat->recv_cmd != pre)
                {
                    memmove(_stat->recv_cmd,pre,total_len - (pre - head));
                    _stat->recv_pos = total_len - (pre - head);
                }
                return 0;
            }
            if(*pre != '*' )    return -1;
            pre++;
            *end = '\0';
            num = atoi(pre);
            if(num == 0) return -1;
        
            tmp_Cmd = (Cmd *)malloc(sizeof(*tmp_Cmd));
            memset(tmp_Cmd,0,sizeof(*tmp_Cmd));
            tmp_Cmd->args_num = num;
            tmp_Cmd->cur_item = 0;
            cmd(_stat) = tmp_Cmd;
            ALL_cmd(_stat);
            if(ResponseCount(_stat) > 0)
                aeCreateFileEvent(eventloop,_stat->fd,AE_WRITABLE,send_response_handle,_stat);
            CmdInsert(_stat,tmp_Cmd);

            end += 2;
            pre = end;
        }
        while(cmd(_stat)->cur_item  < cmd(_stat)->args_num)
        {
            int real_num = 0;

            if(pre >= head + total_len)
            {
                _stat->recv_pos = 0;
                return 0;
            }
            
            if(*pre != '$') return -1;
            end = strstr(end,"\r\n");
            if(end == NULL) 
            {
                if(_stat->recv_cmd != pre)
                {
                    memmove(_stat->recv_cmd,pre,total_len - (pre - head));
                    _stat->recv_pos = total_len - (pre - head);
                }
                return 0;
            }
            pre++;
            *end = '\0';
            real_num = num = atoi(pre);
            num += 1 +strlen(pre) + 2  + 2 ;
            *end = '\r';
            pre--;
            if(num <= 5) return -1;
            cmd(_stat)->item_length[cmd(_stat)->cur_item] = num;
            cmd(_stat)->value[cmd(_stat)->cur_item] = (char *)malloc(num);
            if(pre + num - head > total_len)
            {
                memmove(cmd(_stat)->value[cmd(_stat)->cur_item],pre,total_len - (pre - head));
                cmd(_stat)->item_pos[cmd(_stat)->cur_item] = total_len - (pre - head);
                _stat->recv_pos = 0;
                //cmd(_stat)->cur_item++;
                return 0;
            }
            memmove(cmd(_stat)->value[cmd(_stat)->cur_item],pre,num);
            pre += num;
            end = pre;
            cmd(_stat)->item_pos[cmd(_stat)->cur_item] = num;
            cmd(_stat)->cur_item++;
            if(cmd(_stat)->cur_item > cmd(_stat)->args_num)
                return -1;
        }
        
        
        if(cmd(_stat)->cur_item  >= cmd(_stat)->args_num)
        {
            _stat->cur_cmd_item = NULL;
            ALL_cmd(_stat);
            if(ResponseCount(_stat) > 0)
                aeCreateFileEvent(eventloop,_stat->fd,AE_WRITABLE,send_response_handle,_stat);
        }
        if(end >= head + total_len)
        {
            _stat->recv_pos = 0;
            return 0;
        }

    }

    return 0;
}
void _free_client_cur_cmd(ClientStat *client)
{
    Cmd *tmp_cmd = NULL;
    int pos ;
    
    printf("!!!!!free_client_cur_cmd\n");
    if(!client) return;
    if(cmd(client) == NULL) return;

    for(pos = 0;pos < cmd(client)->args_num ; pos++)
    {
        if(cmd(client)->value[pos] != NULL)
        {
            free(cmd(client)->value[pos]);
            cmd(client)->value[pos] = NULL;
        }
    }
    CmdDeleteHead(client);
    cmd(client) = NULL;
    client->recv_pos = 0;

    return ;
}
void _debug_print_client_cmd(ClientStat * client)
{
    if(!client)  return ;
    
    
    {
        int i ;
        char strings[64];

        Cmd * tmp_cmd = client->recv_cmd_queue_head;
        printf("{\n");
        for(;tmp_cmd ; tmp_cmd = tmp_cmd->next)
        {
            if(tmp_cmd->item_length[tmp_cmd->args_num-1] <= tmp_cmd->item_pos[tmp_cmd->args_num -1])
            {
                for(i = 0; i < tmp_cmd->args_num;i++)
                {
                    memset(strings,0,64);
                    memcpy(strings,tmp_cmd->value[i],tmp_cmd->item_length[i]);
                    printf("%s ",strings);
                }
                printf("\n");
            }
        }
        printf("}\n");
        
    }

    return ;
}
/*
 *  for example:
 *          set name bojianbin
 *          *3\r\n$3\r\nset\r\n$4\r\nname\r\n$9\r\nbojianbin\r\n
 * 
 */
void recv_cmd_handle(struct aeEventLoop *eventloop,int fd,void *clientData,int mask)
{
    int ret ;
    ClientStat *client = (ClientStat *)clientData;
    
    while(1)
    {
    errno = 0;
    if(cmd(client) != NULL && cmd(client)->item_length[cmd(client)->cur_item] != 0 && cmd(client)->item_length[cmd(client)->cur_item] > cmd(client)->item_pos[cmd(client)->cur_item])
    {
        ret  = read(fd,cmd(client)->value[cmd(client)->cur_item] + cmd(client)->item_pos[cmd(client)->cur_item],cmd(client)->item_length[cmd(client)->cur_item] - cmd(client)->item_pos[cmd(client)->cur_item]);
        if(ret <= 0)
        {
            if(errno != EWOULDBLOCK && errno != EAGAIN)
            {
                server_log(LOG_NOTICE,"disconnect1 %d %s:%d %s\n",ret,client->ip,client->port,strerror(errno));
                clientDelete(client,eventloop);
            }
            return;
        }
        
        cmd(client)->item_pos[cmd(client)->cur_item] += ret;
        if(cmd(client)->item_pos[cmd(client)->cur_item] >= cmd(client)->item_length[cmd(client)->cur_item])
        {
            client->recv_pos = 0;
            cmd(client)->cur_item++;
        }
        if(cmd(client)->cur_item  >= cmd(client)->args_num)
        {
            client->cur_cmd_item = NULL;
            ALL_cmd(client);
            if(ResponseCount(client) > 0)
                aeCreateFileEvent(eventloop,client->fd,AE_WRITABLE,send_response_handle,client);
        }
        //_debug_print_client_cmd(client);

    }
    else
    {
        int check_ret ;

        ret = read(fd,&(client->recv_cmd[client->recv_pos]) ,CLIENT_RECV_CMD_LENGTH - client->recv_pos);
        
        if(ret <= 0)
        {
            if(errno != EWOULDBLOCK && errno != EAGAIN)
            {
                server_log(LOG_NOTICE,"disconnect2 %d %s:%d %s ** %d\n",ret,client->ip,client->port,strerror(errno),CLIENT_RECV_CMD_LENGTH - client->recv_pos);
                clientDelete(client,eventloop);
            }
            return;
            
        }
        
        client->recv_pos += ret ;
        check_ret = _check_cmd_complete2(eventloop,client,client->recv_pos);
        //_debug_print_client_cmd(client);
        if(check_ret < 0){
            _free_client_cur_cmd(client);
            //_debug_print_client_cmd(client);
        }

    }
    }
    

    return ;
}
 //          *3\r\n$3\r\nset\r\n$4\r\nname\r\n$9\r\nbojianbin\r\n
void send_response_handle(struct aeEventLoop *eventloop,int fd,void *clientData,int mask)
{
    Response *tmp = NULL;
    int cur_item = 0;
    ClientStat *client = (ClientStat *)clientData;
    if(ResponseCount(client) <= 0) 
    {
        aeDeleteFileEvent(eventloop,fd,AE_WRITABLE);
        return;
    }
   
    for(tmp = ResponseHead(client);tmp;)
    {
        cur_item = tmp->cur_item;
    
        if(tmp->item_pos[cur_item] >= tmp->item_length[cur_item])
        {
            tmp->cur_item += 1;
            cur_item = tmp->cur_item;
        }
        if(tmp->cur_item < tmp->args_num)
        {
            int ret ;
            errno = 0;
            ret = write(fd,tmp->value[cur_item],tmp->item_length[cur_item] - tmp->item_pos[cur_item]);
            if(ret < 0 && errno != EAGAIN)
            {
                clientDelete(client,eventloop);
                return;
            }else if(ret < 0 && errno == EAGAIN)
            {
                return;
            }else
            {
                tmp->item_pos[cur_item] += ret;
            }
             
        }else
        {
            //printf("we send %s %d\n",tmp->value[1],ResponseCount(client));
            ResponseDeleteHead(client);
            tmp = ResponseHead(client);
        }
    }
    
    aeDeleteFileEvent(eventloop,fd,AE_WRITABLE);


    return;
}
int init_tcp_server()
{
    init_dict(DEFAULT_DICT_SIZE);
    return 0;
}

