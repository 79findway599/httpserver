#include <string.h>

#include "ad_method.h"
#include "ad_utils.h"

unsigned int _ad_method_char_lookup(const char *str, unsigned int len)
{
  const unsigned char method_index_lookup[] =
    {
      13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
      13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
      13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
      13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
      13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
      13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
      13, 13, 13, 13, 13, 13, 13,  5,  0, 13,
      13,  0,  0, 13, 13, 13, 13, 13, 13,  0,
       5, 13, 13, 13,  0, 13, 13, 13, 13, 13,
      13, 13, 13, 13, 13, 13, 13, 13, 13,  5,
       0, 13, 13,  0,  0, 13, 13, 13, 13, 13,
      13,  0,  5, 13, 13, 13,  0, 13, 13, 13,
      13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
      13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
      13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
      13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
      13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
      13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
      13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
      13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
      13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
      13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
      13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
      13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
      13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
      13, 13, 13, 13, 13, 13
    };
  return len + method_index_lookup[(unsigned char)str[0]];
}


int ad_method_is_valid(const char *input)
{
    const char * const method_list[] =
    {
      NULL, NULL, NULL,
      "GET",
      "HEAD",
      "TRACE",
      "DELETE",
      "OPTIONS",
      "PUT",
      "POST",
      "PATCH",
      NULL,
      "CONNECT"
    };

    if(input)
    {
        unsigned int input_len = strlen(input);
        if (input_len <= _MAX_METHOD_LENGTH && input_len >= _MIN_METHOD_LENGTH)
        {
            int key = _ad_method_char_lookup(input, input_len);

            if (key <= _MAX_HASH_VALUE && key >= 0)
            {
                const char *method = method_list[key];

                if (!ad_utils_strcmp_ic(method, input))
                  return 1;
            }
        }
    }

    return 0;
}