/*************************************************************************
	> File Name: cmd.c
	> Author: 
	> Mail: 
	> Created Time: Tue 06 Dec 2016 01:59:49 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/if.h>

#include"tcp_server.h"
#include"cmd.h"

uint32_t do_protocol(ClientStat * arg)
{

    return SUCCESS_CMD;
}

