/*************************************************************************
	> File Name: log.h
	> Author: 
	> Mail: 
	> Created Time: 2016年08月20日 星期六 18时45分01秒
 ************************************************************************/

#ifndef _LOG_H
#define _LOG_H

#define LOG_FILE_NAME "/var/run/ae_main.log"
typedef enum
{
    LOG_NOTICE = 0,
    LOG_ALERT,
    LOG_ERROR
}LOG_LEVEL;
extern LOG_LEVEL global_log_level;

#define server_log(level,str, ...) \
    {\
        if(level >= global_log_level) \
            save_log(str,##__VA_ARGS__);\
    };

void init_log_level(LOG_LEVEL );
int save_log(const char *str, ...);

#endif
