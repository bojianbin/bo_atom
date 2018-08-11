/*************************************************************************
	> File Name: cmd.c
	> Author: 
	> Mail: 
	> Created Time: Tue 06 Dec 2016 01:59:49 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"tcp_server.h"
#include"cmd.h"
#include"dict.h"

unsigned int myHashfunc(const void *);
int myKeyconpare(void *,const void *,const void *);
void myKeyfree(void *pri,void *key);
void myValuefree(void *pri,void *key);

dict * Dict = NULL;
dictType dicttype = 
{
    myHashfunc,NULL,NULL,myKeyconpare,myKeyfree,myValuefree
};
char *set_ok_str = "*1\r\n$2\r\nOK\r\n";
char *set_err_str = "*1\r\nS3\r\nERR\r\n";


int init_dict(unsigned long size)
{
    if(Dict)
        return 0;
    Dict = dictCreate(&dicttype,NULL);
    return dictExpand(Dict,size);
}
Response * _get_ok_response()
{
    Response * set_ok = NULL;
    set_ok = (Response *)malloc(sizeof(*set_ok));
    set_ok->args_num = 1;
    set_ok->value[0] = set_ok_str;
    set_ok->item_length[0] = strlen(set_ok->value[0]);
    set_ok->item_pos[0] = 0;
    set_ok->cur_item = 0;
    set_ok->next = NULL;

    return set_ok;
}
Response * _get_err_response()
{
    Response * set_err = NULL;

    set_err = (Response *)malloc(sizeof(*set_err));
    set_err->args_num = 1;
    set_err->value[0] = set_err_str;
    set_err->item_length[0] = strlen(set_err->value[0]);
    set_err->item_pos[0] = 0;
    set_err->cur_item = 0;
    set_err->next = NULL;
    
    return set_err;
}
void _debug_print_string(char *str)
{
    //example: *3\r\nset\r\n
    char *pre = str;
    char *end = NULL;

    if(!str) return;

    pre = strstr(pre,"\r\n");
    if(!pre) 
    {
        printf("_debug_print_string error\n");
        return;
    }
    pre += 2;
    end = pre;
    end = strstr(end,"\r\n");
    if(!end) 
    {
        printf("_debug_print_string error\n");
    }
    *end = '\0';
    printf("%s ",pre);
    *end = '\r';

    return;
}
char *num_1 = "*1\r\n";
Response * SET_cmd(Cmd *cmd)
{
    int fixed_set_cmds = 3;
    DictItem *item_key = NULL;
    DictItem *item_value = NULL;

    if(cmd->args_num != 3)
        return _get_err_response();
#ifdef DEBUG
    {
        printf("**In SET_cmd function:  ");
        _debug_print_string(cmd->value[0]);
        _debug_print_string(cmd->value[1]);
        _debug_print_string(cmd->value[2]);
        printf("\n");
    }
#endif
    item_key = (DictItem *)malloc(sizeof(*item_key));
    item_value = (DictItem *)malloc(sizeof(*item_value));
    
    item_key->item = cmd->value[1];
    item_key->length = cmd->item_length[1];
    item_value->item = cmd->value[2];
    item_value->length = cmd->item_length[2];


    free(cmd->value[0]);
    if(dictAdd(Dict,item_key,item_value) < 0)
    {
        dictReplace(Dict,item_key,item_value);
        Dict->type->keyDestructor(NULL,item_key);
        item_key = NULL;
    }
    return _get_ok_response(); 
}
Response* GET_cmd(Cmd *cmd)
{
    DictItem *item_key = NULL;
    dictEntry *entry = NULL;
    Response *response = NULL;
   
#ifdef DEBUG
    {
        printf("In GET cmd function:  ");
        _debug_print_string(cmd->value[0]);
        _debug_print_string(cmd->value[1]);
        printf("\n");
    }
#endif
    if(!cmd) return _get_err_response();
    if(cmd->args_num != 2 && cmd->args_num != 3) return _get_err_response();
    item_key = (DictItem *)malloc(sizeof(*item_key));
    item_key->item = cmd->value[1];
    item_key->length = cmd->item_length[1];
    free(cmd->value[0]);
    
    entry = dictFind(Dict,item_key);
    myKeyfree(NULL,item_key);
    if(!entry)
    {
        return _get_err_response();
    }else
    {
        response = (Response *)malloc(sizeof(*response));
        memset(response,0,sizeof(*response));
        response->args_num = 2;
        response->value[0] = num_1;
        response->item_length[0] = strlen(num_1);
        response->value[1] = ((DictItem *)(entry->v.val))->item;
        response->item_length[1] = ((DictItem *)(entry->v.val))->length;

        return response;
    }
}
Response * ALL_cmd(ClientStat *client)
{
    Cmd * tmp = NULL;

    if(client == NULL)  return NULL;
    for(tmp = CmdHead(client); tmp ; tmp = tmp->next)
    {
        Response *res = NULL;
        if(CmdComplete(client,tmp) && tmp->args_num > 0 && cmd(client) != tmp)
        {

            if(memcmp(tmp->value[0],"$3\r\nSET\r\n",tmp->item_length[0]) == 0 || \
                    memcmp(tmp->value[0],"$3\r\nset\r\n",tmp->item_length[0]) == 0)
            {
                res = SET_cmd(tmp);
            }
            if(memcmp(tmp->value[0],"$3\r\nGET\r\n",tmp->item_length[0]) == 0 || \
                    memcmp(tmp->value[0],"$3\r\nget\r\n",tmp->item_length[0]) == 0)
            {
                res = GET_cmd(tmp);
            }

            ResponseInsert(client,res);
        }
    }

    while(CmdHead(client) && CmdComplete(client,CmdHead(client)))
    {
        CmdDeleteHead(client);
    }

    return NULL;
}

unsigned int myHashfunc(const void *arg)
{
    DictItem * item = NULL;

    item = (DictItem *)arg;
    return dictGenHashFunction(item->item,item->length);
}

//0 :   not same
//1:    same
int myKeyconpare(void *privdata,const void * key1,const void *key2)
{
    DictItem *item1,*item2 ;
    
    if( !key1 || !key2)
        return 0;
    
    item1 = (DictItem *)key1;
    item2 = (DictItem *)key2;
    if(item1->length != item2->length)
        return 0;
    if(memcmp(item1->item,item2->item,item1->length) == 0)
        return 1;
    
    return 0;
}
void myKeyfree(void *pri,void *key)
{
    DictItem * tmp = (DictItem *)key;
    if(!tmp) return ;

    free(tmp->item);
    free(tmp);

    return;
}
void myValuefree(void *pri,void *value)
{
    DictItem * tmp = (DictItem *)value;
    
    if(!tmp) return ;

    free(tmp->item);
    free(tmp);

    return;
}
