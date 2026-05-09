//#pragma once
#include "stdlib.h"

#include "qglobe_base_global.h"

QGLOBE_BASE_EXPORT void *	qGlobeMemMalloc(size_t size);
QGLOBE_BASE_EXPORT void *	qGlobeMemRealloc(void * ptr, size_t size);
QGLOBE_BASE_EXPORT void *	qGlobeMemCalloc(size_t nelem,   size_t elsize);
QGLOBE_BASE_EXPORT int	qGlobeMemFree(void const * ptr);
QGLOBE_BASE_EXPORT char *	qGlobeMemStringCpy(const char * s1);

