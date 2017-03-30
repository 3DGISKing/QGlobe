//#pragma once
#include "stdlib.h"

#include "gdem_base_global.h"

GDEM_BASE_EXPORT void *	gdmMemMalloc(size_t size);
GDEM_BASE_EXPORT void *	gdmMemRealloc(void * ptr, size_t size);
GDEM_BASE_EXPORT void *	gdmMemCalloc(size_t nelem,   size_t elsize);
GDEM_BASE_EXPORT int	gdmMemFree(void const * ptr);
GDEM_BASE_EXPORT char *	gdmMemStringCpy(const char * s1);

