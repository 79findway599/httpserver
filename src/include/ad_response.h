#ifndef AD_RESPONSE_H
#define AD_RESPONSE_H

#include <setjmp.h>

//成功
#define AD_RESPONSE_HTTP_OK "HTTP/1.0 200 OK" CLRF CLRF
//400错误的请求
#define AD_RESPONSE_CLIENT_BAD_REQUEST "HTTP/1.0 400 Bad Request" CLRF CLRF
//404错误
#define AD_RESPONSE_CLIENT_NOT_FOUND "HTTP/1.0 404 Not Found" CLRF CLRF
//501 内部错误
#define AD_RESPONSE_SERVER_NOT_IMPLEMENTED "HTTP/1.0 501 Not Implemented" CLRF CLRF

//接收请求
void ad_response_receive(int client, char *buff, size_t buff_len, jmp_buf error_jmp);

void ad_response_send(int client, char *response, jmp_buf error_jmp);

void ad_response_sendfile(int client, int file, jmp_buf error_jmp);

#endif