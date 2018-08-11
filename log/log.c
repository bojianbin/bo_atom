/*************************************************************************
	> File Name: log.c
	> Author: 
	> Mail: 
	> Created Time: 2016年08月20日 星期六 18时44分55秒
 ************************************************************************/

#include<stdio.h>
#include<sys/time.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdarg.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

#include"log.h"

LOG_LEVEL global_log_level = LOG_NOTICE;

void  init_log_level(LOG_LEVEL level)
{
    global_log_level = level;

    return ;
}
int save_log(const char *str, ...)
{
    char time_buf[200] = {0};
    time_t          tmp_time;
    struct tm       *tmp_tm;
    struct timeval  tmp_val;
    FILE *fd = NULL;
    va_list  list;

    tmp_time = time(NULL);
    tmp_tm = localtime(&tmp_time);
    gettimeofday(&tmp_val,NULL);
    sprintf(time_buf,"%d-%d-%d %d:%d:%d.%03d ",tmp_tm->tm_year+1900,tmp_tm->tm_mon+1,tmp_tm->tm_mday,tmp_tm->tm_hour,tmp_tm->tm_min,tmp_tm->tm_sec,(int)tmp_val.tv_usec);

    fd = fopen(LOG_FILE_NAME,"a+");
    if(fd == NULL)
    {
        //printf("open server.log error %s\n",strerror(errno));
        return -1;
    }
    fprintf(fd,"%s",time_buf);
    memset(time_buf,0,200);
    va_start(list,str);
    vsprintf(time_buf,str,list);
    fprintf(fd,"%s",time_buf);
    va_end(list);

    fclose(fd);
    fd = NULL;

    return 0;
}
