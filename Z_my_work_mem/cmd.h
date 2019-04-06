/*************************************************************************
	> File Name: cmd.h
	> Author: 
	> Mail: 
	> Created Time: Tue 06 Dec 2016 02:00:03 PM CST
 ************************************************************************/

#ifndef _CMD_H
#define _CMD_H
#include "tcp_server.h"

Response * ALL_cmd(ClientStat * );

int init_dict(unsigned long );
int init_cmd_data();

#endif
