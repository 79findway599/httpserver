#include <stdlib.h>

#include "ad_thread_pool.h"


ad_thread_pool *ad_thread_pool_construct(ad_thread_pool_mutex *mutex, 
                                         ad_thread_pool_cond *cond_var, 
                                         ad_queue *request_queue)
{
    int i;
    ad_thread_pool *thread_pool = malloc(sizeof(ad_thread_pool));
 
    THREAD_QUEUE(thread_pool) = ad_queue_construct(mutex, cond_var);

    REQUEST_QUEUE(thread_pool) = request_queue;

    //循环创建线程
    for (i = 0; i < AD_THREAD_POOL_MIN_THREADS; i++)
    {
        ad_thread_pool_create_thread(thread_pool);
    }

    return thread_pool;
}

void ad_thread_pool_create_thread(ad_thread_pool *thread_pool)
{
    ad_thread_attr attr;
    ad_thread *new_thread = malloc(sizeof(ad_thread));
    ad_thread_parameters *parameters = malloc(sizeof(ad_thread_parameters));

    MUTEX(parameters) = MUTEX(REQUEST_QUEUE(thread_pool));//把任务的锁赋值给表示线程参数的锁
    COND(parameters) = COND(REQUEST_QUEUE(thread_pool));
    REQUEST_QUEUE(parameters) = REQUEST_QUEUE(thread_pool);
        
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);//设置线程分离属性
    pthread_create(new_thread, &attr, ad_thread_handle_requests, (void *) parameters);

    ad_queue_push(THREAD_QUEUE(thread_pool), (void *) new_thread);
}


int ad_thread_pool_delete_thread(ad_thread_pool *thread_pool)
{
    ad_thread *thread = (ad_thread *) ad_queue_pop(THREAD_QUEUE(thread_pool));

    if(thread)
    {
        pthread_cancel(*thread);
        free(thread);
        return 1;
    }
    return 0;
}

void ad_thread_pool_destruct(ad_thread_pool *thread_pool)
{
    while(ad_thread_pool_delete_thread(thread_pool));

    ad_queue_destruct(THREAD_QUEUE(thread_pool));
    free(thread_pool);
}