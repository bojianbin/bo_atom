/*************************************************************************
	> File Name: defines.h
	> Author: 
	> Mail: 
	> Created Time: Fri 18 Nov 2016 02:36:41 PM CST
 ************************************************************************/

#ifndef _DEFINES_H
#define _DEFINES_H

#define SERVER_PORT 9001
#define CLIENT_RECV_CMD_LENGTH 64
#define CLIENT_SEND_RESPONSE_LENGTH 512

#define CLIENT_RECV_CMD_MAX (5 * 1024 * 1024)
#define CLIENT_SEND_RESPONSE_MAX (5 * 1024)

#define CMD_ARGS_MAX 512

#define DEFAULT_DICT_SIZE 50

#define PID_FILE_NAME "/var/run/ae_main.pid"











#endif
