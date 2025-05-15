#ifndef KS_UTILS_H
#define KS_UTILS_H

#include <stdio.h>
#include <stdlib.h>

bool malloc_failure_guard(void *p);
bool malloc_failure_janitor(void *p, uint8_t argc, ...);

#endif
