#ifndef AD_HTTP_REQUEST_H
#define AD_HTTP_REQUEST_H

#define CLRF "\r\n"
//http最大请求长度
#define AD_HTTP_REQUEST_MAX_SIZE 4096

typedef struct {
    char *name;
    char *field;
}ad_http_header;

typedef struct {
    char *method;   //方法
    char *uri;      //url
    char *version;  //版本
    ad_http_header **headers;
}ad_http_request;

#define METHOD(r)   ((r) -> method)
#define URI(r)      ((r) -> uri)
#define VERSION(r)  ((r) -> version)
#define HEADERS(r)  ((r) -> headers)

//解析
ad_http_request *ad_http_request_parse(char *request);

void ad_http_request_free(ad_http_request *http_request);

#endif