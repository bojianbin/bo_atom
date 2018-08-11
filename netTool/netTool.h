/*************************************************************************
	> File Name: netTool.h
	> Author: 
	> Mail: 
	> Created Time: Wed 13 Dec 2017 04:41:17 PM CST
 ************************************************************************/

#ifndef _NETTOOL_H
#define _NETTOOL_H

int readn(int fd,uint8_t * buf,int size,int max_null_milliseconds);
int writen(int fd,uint8_t * buf,int size,int max_null_milliseconds);


#endif
