#include <string.h>
#include <stdlib.h>

#include "ad_http_request.h"
#include "ad_utils.h"


ad_http_request *ad_http_request_parse(char *request)
{
    int i = 0;
    char *end, *src = request;
    char **tokens = NULL;
    char **buffer = NULL;
    size_t substr_len;
    ad_http_request *http_request = NULL;

    if (request != NULL && strstr(request, "\r\n\r\n") != NULL && strlen(request) > strlen(CLRF)) 
    {
        http_request = malloc(sizeof(ad_http_request));
        HEADERS(http_request) = NULL;
        //解析
        for (i = 0; (end = strstr(src, CLRF)); i++)
        {
            //追加堆内存大小
            buffer = realloc(buffer, sizeof(char *) * (i + 2));

            substr_len = end - src;

            buffer[i] = malloc(sizeof(char) * (substr_len + 1));
            strncpy(buffer[i], src, substr_len);
            buffer[i][substr_len] = '\0';

            src = end + strlen(CLRF);
        }

        buffer[i] = NULL;

        if (buffer[0])
        {
            //分割字符串
            tokens = ad_utils_split_str(buffer[0], " ");

            METHOD(http_request) = tokens[0];

            URI(http_request) = tokens[1];

            VERSION(http_request) = tokens[2];

            free(tokens);
        }

        for (i = 0; buffer[i]; i++) 
        {
            free(buffer[i]);
        }
        free(buffer);

    }

    return http_request;
}


void ad_http_request_free(ad_http_request *http_request) 
{
    size_t i;

    free(METHOD(http_request));
    free(URI(http_request));
    free(VERSION(http_request));

    if(HEADERS(http_request))
    {
        for (i = 0; HEADERS(http_request)[i]; i++)
        {
            free(HEADERS(http_request)[i]);
        }
    }

    free(http_request);
}