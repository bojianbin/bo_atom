// Example.

#include "jsw_rbtree.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct centroid {
  double            mean;
  int               count;
} centroid_t;


static int centroid_cmp(const void *p1, const void *p2)
{
	centroid_t *centroid1, *centroid2;

	centroid1 = (centroid_t*)p1;
	centroid2 = (centroid_t*)p2;

	if (centroid1->mean > centroid2->mean)
		return 1;

	else if (centroid1->mean < centroid2->mean)
		return -1;

	return 0;
}

static void *centroid_dup(void *p)
{
	void *dup_p;

	dup_p = calloc(1, sizeof(struct centroid));
	memmove(dup_p, p, sizeof(struct centroid));

	return dup_p;
}

static void centroid_rel(void *p)
{
	free(p);
}



int main()
{
    int i = 0 ;
    jsw_rbtree_t * head;
    jsw_rbtrav_t * list;
    centroid_t * tmp ;

    head = jsw_rbnew(centroid_cmp,centroid_dup,centroid_rel);

    for(i = 0 ; i < 10 ; i++)
    {
        centroid_t item ;
        item.mean = i;
        item.count = i + 1;
        jsw_rbinsert(head,&item);
    }

    {
        /*
        *   Stop working if we inserted a node. This
        *   check also disallows duplicates in the tree
        */
        int outcome = 0;
        centroid_t item ;
        item.mean = 5;
        item.count = 55;
        outcome = jsw_rbinsert(head,&item);
        printf("$$$%d\n",outcome);
    }
    
    list = jsw_rbtnew();
    tmp = jsw_rbtfirst(list,head);
    while(tmp)
    {
        printf("%d\n",tmp->count);
        tmp = jsw_rbtnext(list);
    }

    return 0;
}
