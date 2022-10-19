#include "ad_server.h"

int main(int argc, char *argv[]) 
{
    int port = argc > 1 ? atoi(argv[1]) : 9229;

    return ad_server_listen(port);//服务端创建socket并监听
}