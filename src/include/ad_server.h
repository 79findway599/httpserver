#ifndef AD_SERVER_H
#define AD_SERVER_H

#include <setjmp.h>//类似goto，完成非局部跳转函数库

#define AD_SERVER_CONNECTION_BACKLOG 128//监听时队列最大长度

void ad_server_terminate(int signum);//信号处理函数

int  ad_server_listen(unsigned short int server_port);//创建池且建立socket

void ad_server_answer(int client_socket, jmp_buf error_jmp);//服务器应答

#endif