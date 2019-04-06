/*************************************************************************
	> File Name: base_type.h
	> Author: 
	> Mail: 
	> Created Time: Saturday, April 06, 2019 PM08:23:25 CST
 ************************************************************************/

#ifndef _BASE_TYPE_H
#define _BASE_TYPE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#ifndef WRITE_ONCE
#define WRITE_ONCE(a,b) (a = b)
#endif

#ifndef READ_ONCE
#define READ_ONCE(a) (a)
#endif

#ifndef unlikely
#define unlikely(a) (a)
#endif

#ifndef likely
#define likely(a) (a)
#endif

#ifndef EXPORT_SYMBOL
#define EXPORT_SYMBOL(a) 
#endif

#define __force
#define __user
#define __must_check
#define __cold

#define LIST_POISON1 NULL
#define LIST_POISON2 NULL

int rcu_scheduler_active;
static inline int rcu_lockdep_current_cpu_online(void)
{
	return 1;
}
static inline int rcu_is_cpu_idle(void)
{
	return 1;
}
static inline bool rcu_is_watching(void)
{
	return false;
}
#define rcu_assign_pointer(p, v) ((p) = (v))
#define RCU_INIT_POINTER(p, v) p=(v)


#endif
