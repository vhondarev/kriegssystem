#include "utils.h"
#include "messages.h"
#include <stdarg.h>
#include <stdint.h>

bool malloc_failure_guard(void *p)
{
    if (p == NULL)
    {
        perror(MSG_ERR_MEMORY_FAIL);
        return true;
    }

    return false;
}

bool malloc_failure_janitor(void *p, uint8_t argc, ...)
{
    if (p == NULL)
    {
        if (argc > 0)
        {
            va_list args;
            va_start(args, argc);
            for (uint8_t i = 0; i < argc; i++)
            {
                free(va_arg(args, void *));
            }
            va_end(args);
        }
        perror(MSG_ERR_MEMORY_FAIL);
        return true;
    }

    return false;
}
