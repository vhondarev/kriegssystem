#include "utils.h"
#include <stdarg.h>
#include <stdint.h>

void malloc_failure_guard(void *p, uint8_t argc, ...)
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
        perror("Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
}
