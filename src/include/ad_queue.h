#ifndef AD_QUEUE_H
#define AD_QUEUE_H

#include <pthread.h>

typedef struct node {
    void *data;
    struct node *next;
}ad_node;//节点

#define DATA(n)     ((n)->data)
#define NEXT(n)     ((n)->next)

//互斥锁及条件变量重定义
typedef pthread_mutex_t ad_queue_mutex;
typedef pthread_cond_t ad_queue_cond;

typedef struct {
    int node_count;     //节点数量
    ad_node *head;      //头节点
    ad_node *last;      //尾节点
    ad_queue_mutex *mutex;//互斥锁
    ad_queue_cond *cond;//条件变量
}ad_queue;//池

#define NODE_COUNT(q) ((q)->node_count)
#define IS_EMPTY(q)   ((q)->node_count ? 0 : 1)
#define HEAD(q)       ((q)->head)
#define LAST(q)       ((q)->last)

#define MUTEX(q) ((q)->mutex)
#define COND(q)  ((q)->cond)

//初始化
ad_queue *ad_queue_construct(ad_queue_mutex *mutex, ad_queue_cond *cond_var);
//入队
void ad_queue_push(ad_queue *queue, void *data);
//出队
void *ad_queue_pop(ad_queue *queue);
//销毁
void ad_queue_destruct(ad_queue *queue);

#endif