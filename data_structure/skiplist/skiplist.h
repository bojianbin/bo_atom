/*************************************************************************
	> File Name: skip.h
	> Author: 
	> Mail: 
	> Created Time: Mon 27 Aug 2018 02:36:31 PM CST
 ************************************************************************/

#ifndef _SKIP_H
#define _SKIP_H


#define ZSKIPLIST_MAXLEVEL 32 /* Should be enough for 2^32 elements */
#define ZSKIPLIST_P 0.25      /* Skiplist P = 1/4 */

typedef int   (*cmp_func) ( void *p1, void *p2 );
typedef int   (*free_func) ( void *p1);


/* Struct to hold a inclusive/exclusive range spec by score comparison. */
typedef struct 
{
    void * min, *max;
    int minex, maxex; /* are min or max exclusive? */
} zrangespec;


typedef struct zskiplistNode 
{
    void *obj;
    struct zskiplistNode *backward;
    struct zskiplistLevel 
    {
        struct zskiplistNode *forward;
        unsigned int span;
    } level[];

} zskiplistNode;

typedef struct zskiplist 
{
    struct zskiplistNode *header, *tail;
    cmp_func  cmp;
    free_func free_obj;
    unsigned long length;
    int level;

} zskiplist;



zskiplist *zslCreate(cmp_func cmp,free_func _free); 

void zslFree(zskiplist *zsl); 

zskiplistNode *zslInsert(zskiplist *zsl, void *obj); 

int zslDelete(zskiplist *zsl,void *obj); 

zskiplistNode *zslFirstInRange(zskiplist *zsl, zrangespec *range); 

zskiplistNode *zslLastInRange(zskiplist *zsl, zrangespec *range); 

unsigned long zslDeleteRange(zskiplist *zsl, zrangespec *range); 

unsigned long zslDeleteRangeByRank(zskiplist *zsl, unsigned int start, unsigned int end); 

unsigned long zslGetRank(zskiplist *zsl, void *o); 

zskiplistNode* zslGetElementByRank(zskiplist *zsl, unsigned long rank); 


#endif
