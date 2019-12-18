/*************************************************************************
	> File Name: netTool.h
	> Author: 
	> Mail: 
	> Created Time: Wed 13 Dec 2017 04:41:17 PM CST
 ************************************************************************/

#ifndef _NETTOOL_H
#define _NETTOOL_H

typedef enum {
	NetTool_Readable = 1<<0,
	NetTool_writeable = 1<<1,
	NetTool_exceptable = 1<<2,
}NetTool_sock_status;

int readn(int fd,uint8_t * buf,int size,int max_null_milliseconds);
int writen(int fd,uint8_t * buf,int size,int max_null_milliseconds);
int eventable(int fd , NetTool_sock_status ask,int max_null_milliseconds);

#endif
