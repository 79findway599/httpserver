#include <stdio.h>
//互斥锁变量静态初始化依赖宏
#define __USE_GNU
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include "ad_server.h"
#include "ad_http_request.h"
#include "ad_response.h"
#include "ad_utils.h"
#include "ad_method.h"

#include <pthread.h>

#include "ad_queue.h"
#include "ad_thread_pool.h"

//sig_atomic_t 对变量进行操作或赋值时保证是原子操作
//volatile 告诉编辑器，每次使用时都需要去内存重新读值
volatile sig_atomic_t ad_server_terminating = 0;//服务器终止开关，0表示正常，1表示终止


void ad_server_terminate(int signum) //信号处理函数
{ 
    ad_server_terminating = 1;//服务器终止开关，1表示终止
    fprintf(stdout, "\nAbaddon HTTP server is terminating please wait!\n");
    fflush(stdout);
}


void ad_server_answer(int client_socket, jmp_buf error_jmp)
{
    int requested_file;
    char path[512];
    char buffer[AD_HTTP_REQUEST_MAX_SIZE];
    ad_http_request *http_request = NULL;//指向一个http的请求报文

	ad_response_receive(client_socket, buffer, AD_HTTP_REQUEST_MAX_SIZE, error_jmp);

    http_request = ad_http_request_parse(buffer);//解析出请求行的信息

    if (http_request == NULL || METHOD(http_request) == NULL || !ad_method_is_valid(METHOD(http_request)))
    {
        //如果不是http请求
        ad_http_request_free(http_request);
        ad_response_send(client_socket, AD_RESPONSE_CLIENT_BAD_REQUEST, error_jmp);
    }
    else if (ad_utils_strcmp_ic(METHOD(http_request), "GET"))
    {
        //如果不是get请求
        ad_http_request_free(http_request);
        ad_response_send(client_socket, AD_RESPONSE_SERVER_NOT_IMPLEMENTED, error_jmp);
    }
    else if (!ad_utils_strcmp_ic(METHOD(http_request),"GET"))
    {
        //./htdocs/index.html
        sprintf(path, "./htdocs%s", URI(http_request));
        if (ad_utils_is_directory(path))
        {
            strcat(path, "index.html");
        }

        // sprintf(path, "./html%s", URI(http_request));
        // if (ad_utils_is_directory(path))
        // {
        //     strcat(path, "demo.html");
        // }

        ad_http_request_free(http_request);

        if ((requested_file = open(path, O_RDONLY)) == -1)
        {
            perror(path);
            ad_response_send(client_socket, AD_RESPONSE_CLIENT_NOT_FOUND, error_jmp);
        }
        else 
        {
            ad_response_send(client_socket, AD_RESPONSE_HTTP_OK, error_jmp);

            ad_response_sendfile(client_socket, requested_file, error_jmp);

            close(requested_file);
        }

    }
    

    shutdown(client_socket, SHUT_WR);

	ad_response_receive(client_socket, buffer, AD_HTTP_REQUEST_MAX_SIZE, error_jmp);

    close(client_socket);
}


int ad_server_listen(unsigned short int server_port)
{
    int  server_socket = -1;
    int  client_socket = -1;
    int *client_ptr = NULL;//存储客户端 socket 句柄内存首地址

    struct sigaction sig_struct;
	
    socklen_t server_len, client_len;
    struct sockaddr_in server_addr, client_addr;

    ad_queue *request_queue;//任务队列
    ad_thread_pool *thread_pool;//线程池

    //互斥锁及条件变量静态初始化
	ad_queue_mutex request_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;//嵌套锁
    ad_thread_pool_mutex pool_mutex= PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

    ad_queue_cond add_request_cond = PTHREAD_COND_INITIALIZER;
    ad_thread_pool_cond add_thread_cond = PTHREAD_COND_INITIALIZER;

    signal(SIGPIPE, SIG_IGN);//忽略信号

    sigemptyset(&sig_struct.sa_mask);
    sig_struct.sa_handler = ad_server_terminate;
    sig_struct.sa_flags = 0;
	sigaction(SIGINT, &sig_struct, NULL);

    request_queue = ad_queue_construct(&request_mutex, &add_request_cond);//任务池对象初始化完成
    thread_pool = ad_thread_pool_construct(&pool_mutex, &add_thread_cond, request_queue);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_len = sizeof(server_addr);

	memset(&server_addr, 0, server_len);
    server_addr.sin_port = htons(server_port);
    server_addr.sin_family = AF_INET; 
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 

    if((bind(server_socket, (struct sockaddr *)&server_addr, server_len)) == -1)
    {
        perror("Abaddon HTTP server couldn't start");
        return 1;
    }

    listen(server_socket, AD_SERVER_CONNECTION_BACKLOG);
    printf("Abaddon HTTP server is listening on port %d\n", server_port);

    while (!ad_server_terminating)
    {
        client_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &client_len);

        if (client_socket == -1) 
        { 
            if(errno == EINTR) { /* The server received SIGINT */ }
            else               { perror("Client connection wasn't established\n"); }
        }
        else
        { 
            client_ptr = malloc(sizeof(int));
            *client_ptr = client_socket;
            //新的连接句柄压入任务队列
            ad_queue_push(request_queue, (void *)client_ptr); 

            pthread_cond_signal(COND(THREAD_QUEUE(thread_pool)));
        }
    }
    
    close(server_socket);

    ad_queue_destruct(request_queue);
    ad_thread_pool_destruct(thread_pool);
    
    printf("Abaddon HTTP server terminated successfully\n");

    return 0;
}