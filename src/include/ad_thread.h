#ifndef AD_THREAD_H
#define AD_THREAD_H

#include <pthread.h>
#include <setjmp.h>
#include "ad_queue.h"

typedef struct {
    void *data;
    jmp_buf error_jmp;//表示程序上下文件的结构，用来提供跳转点
}ad_thread_request;

typedef struct {
    ad_queue_mutex *mutex;
    ad_queue_cond *cond;
    ad_queue *request_queue;
}ad_thread_parameters;

#define REQUEST_QUEUE(p) ((p) -> request_queue)

#define VOIDPTR_TO_INT(ptr) (*((int *) (ptr)))//把参数强制转换为int类型数据

//线程取消，回调
void ad_thread_cancel_cleanup(void *mutex);

//任务处理
void ad_thread_handle_requests_hook(ad_thread_request *request);

//线程函数
void *ad_thread_handle_requests(void *thread_parameters);

#endif