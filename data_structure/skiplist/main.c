/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: Mon 27 Aug 2018 02:36:40 PM CST
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"skiplist.h"

#define NUM_DATA  10
struct Person
{
    int random;
    int data;
};
int cmp1(void *_p1,void *_p2)
{
    struct Person *p1 = (struct Person *)_p1;
    struct Person *p2 = (struct Person *)_p2;

    if(p1 == NULL && p2 == NULL)
        return 0;
    if(p2 == NULL || p1->random > p2->random)
    {
        return 1;
    }
    if(p1 == NULL || p1->random < p2->random)
    {
        return -1;
    }
    if(p1->random == p2->random)
    {
        return 0;
    }

    if(p1->data > p2->data)
    {
        return 1;
    }
    if(p1->data < p2->data)
    {
        return -1;
    }
    return 0;
}
int free1(void *data)
{
    if(data)
    {
        free(data);
    }

    return 0;
}
int main()
{
    int i ;
    struct Person tmp_data[NUM_DATA];
    struct Person tmp_data2[NUM_DATA];
    
    zskiplist * list = zslCreate(cmp1,free1);
    for(i = 0 ; i < NUM_DATA ; i++)
    {
        struct Person *item;
        item = (struct Person *)malloc(sizeof(*item));
        item->random = rand() & 0xFF ;
        item->data = i ;
        zslInsert(list,item);
        tmp_data[i].random = item->random;
        tmp_data[i].data = i;
        printf("%d ",tmp_data[i].random);
    }
    printf("\n");

    {
        zskiplistNode *node;
        int i;
        for(i = 0 ; i < NUM_DATA ; i++)
        {
            node = zslGetElementByRank(list,i+1);
            if(node)
            {
                tmp_data2[i].random = ((struct Person *)node->obj)->random; 
                tmp_data2[i].data = ((struct Person *)node->obj)->data; 
                printf("%d ",((struct Person *)node->obj)->random);
            }
        }
        printf("\n");
    }
    
    {
        zrangespec range; 
        zskiplistNode *node;
        struct Person item1;
        struct Person item2;
        item1.random = tmp_data2[1].random; 
        item1.data = tmp_data2[1].data; 
        item2.random = tmp_data2[6].random; 
        item2.data = tmp_data2[6].data;
        range.min = &item1;
        range.max = &item2;
        range.minex = 0;
        range.maxex = 0;

        node = zslFirstInRange(list,&range);
        if(node)
            printf("%d ",((struct Person *)(node->obj))->random);
        node = zslLastInRange(list,&range);
        if(node)
            printf("%d \n",((struct Person *)(node->obj))->random);
    }


    return 0;
}
