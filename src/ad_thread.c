#include <stdlib.h>

#include "ad_thread.h"
#include "ad_server.h"

void ad_thread_cancel_cleanup(void *mutex)
{
    if (mutex)
    {
        pthread_mutex_unlock((ad_queue_mutex *) mutex);
    }
}


void ad_thread_handle_requests_hook(ad_thread_request *request)
{
    jmp_buf error_jmp;//表示函数调用时的进程上下文结构体
    int jmp_status;

    int client = VOIDPTR_TO_INT(request);
    free(request);

    //setjmp 表示当调用该函数时，进程上下文保存在erro_jmp这个结构变量。
    //直接调用setjmp函数返回0。如果从 longjmp 调用返回，则返回非0值
    jmp_status = setjmp(error_jmp);//设置跳转点  longjmp

    if(jmp_status != 1) {ad_server_answer(client, error_jmp);}
}


void *ad_thread_handle_requests(void *thread_parameters)
{
    void *request = NULL;
    ad_queue_mutex *queue_mutex = MUTEX((ad_thread_parameters *) thread_parameters);
    ad_queue_cond *queue_cond   = COND((ad_thread_parameters *) thread_parameters);
    ad_queue *request_queue     = REQUEST_QUEUE((ad_thread_parameters *) thread_parameters);
    free(thread_parameters);

    //设置线程的取消状态
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    //设置线程的取消类型
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    pthread_cleanup_push(ad_thread_cancel_cleanup, (void *) queue_mutex);

    pthread_mutex_lock(queue_mutex);

    //循环处理任务
    for ( ; ; )
    {
        request = ad_queue_pop(request_queue);//得到一个客户端的socket

        if (request)
        {
            pthread_mutex_unlock(queue_mutex);
            ad_thread_handle_requests_hook(request);

            pthread_mutex_lock(queue_mutex);
        }
        else
        {
            pthread_cond_wait(queue_cond, queue_mutex);
        }
    }

	pthread_cleanup_pop(0);
}