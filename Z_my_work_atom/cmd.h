/*************************************************************************
	> File Name: cmd.h
	> Author: 
	> Mail: 
	> Created Time: Tue 06 Dec 2016 02:00:03 PM CST
 ************************************************************************/

#ifndef _CMD_H
#define _CMD_H
#include "tcp_server.h"
#include "stdint.h"

//return value
#define ERR_CMD 0x01 //erro.other fault
#define SUCCESS_CMD 0x02 //success.

#define SEND_CMD 0x04  //need send data out
#define RECV_CMD 0x08  //need recv data
#define ERR_SENDBUFFERFULL_CMD 0x10 //send buffer full






typedef uint32_t (*device_func)(ClientStat *client,void * data);




uint32_t do_protocol(ClientStat *);


#endif
