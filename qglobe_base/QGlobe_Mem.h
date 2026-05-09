//#pragma once
#include "stdlib.h"

#include "qglobe_base_global.h"

QGLOBE_BASE_EXPORT void *	gdmMemMalloc(size_t size);
QGLOBE_BASE_EXPORT void *	gdmMemRealloc(void * ptr, size_t size);
QGLOBE_BASE_EXPORT void *	gdmMemCalloc(size_t nelem,   size_t elsize);
QGLOBE_BASE_EXPORT int	gdmMemFree(void const * ptr);
QGLOBE_BASE_EXPORT char *	gdmMemStringCpy(const char * s1);

