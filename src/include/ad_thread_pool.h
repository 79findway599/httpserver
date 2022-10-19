#ifndef AD_THREAD_POOL_H
#define AD_THREAD_POOL_H

#include <pthread.h>

#include "ad_queue.h"
#include "ad_thread.h"

//线程池最小线程数
#define AD_THREAD_POOL_MIN_THREADS 5

typedef pthread_t ad_thread;
typedef pthread_attr_t ad_thread_attr;//线程属性

typedef pthread_mutex_t ad_thread_pool_mutex;
typedef pthread_cond_t ad_thread_pool_cond;

typedef struct {
    ad_thread *pool_maintainer; //线程id
    ad_queue *thread_queue;     //线程链表
    ad_queue *request_queue;    //任务链表
}ad_thread_pool;//线程池

#define THREAD_QUEUE(p) ((p)->thread_queue)

//线程池初始化
ad_thread_pool *ad_thread_pool_construct(ad_thread_pool_mutex *mutex, 
                                         ad_thread_pool_cond *cond_var,
                                         ad_queue *request_queue);

//创建线程
void ad_thread_pool_create_thread(ad_thread_pool *thread_pool);

//释放
int ad_thread_pool_delete_thread(ad_thread_pool *thread_pool);

//销毁
void ad_thread_pool_destruct(ad_thread_pool *thread_pool);

#endif