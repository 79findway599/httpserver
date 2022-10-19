#include <string.h>
#include <stdlib.h>

#include "ad_utils.h"

int ad_utils_is_directory(const char *path)
{
    return (strrchr(path, '/') - path) == (strlen(path) - 1);
}

int ad_utils_strcmp_ic(const char *str1, const char *str2)
{
    unsigned char c1, c2;

    while ( str1 && str2 )
    {
        c1 = tolower(*str1++);
        c2 = tolower(*str2++);

        if (c1 != 0 && c1 == c2) { continue; }
        else                     { break; }
    }
    return c1 - c2;
}

char **ad_utils_split_str(const char *str, const char *delims)
{
    const char *p, *s = str;
    char **tokens = NULL;
    size_t i, tkn_len;

    if (str != NULL)
    {
        for (i = 0; (p = strpbrk(s, delims)); i++)
        {
            tokens = realloc(tokens, sizeof(char *) * (i + 3));

            tkn_len = p - s;
            tokens[i] = malloc(sizeof(char) * (tkn_len + 1));
            
            strncpy(tokens[i], s, tkn_len);
            tokens[i][tkn_len] = '\0';
            
            s = p + 1;
        }

        tkn_len = strlen(str) - (s - str);
        tokens[i] = malloc(sizeof(char) * (tkn_len + 1));
        
        strncpy(tokens[i], s, tkn_len);
        tokens[i][tkn_len] = '\0';
        
        tokens[i + 1] = NULL; 
    }

    return tokens;
}