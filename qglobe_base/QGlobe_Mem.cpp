#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "QGlobe_Define.h"
#include "QGlobe_Mem.h"

void
pthr_verror1 (char const *prefix, char const *fmt, va_list ap)
{
  char buf[1024], *p;

  strcpy (buf, "ERROR: ");
  p = strchr (buf, '\0');
  strcpy (p, prefix);
  p = strchr (p, '\0');
  *p++ = ' ';

  vsprintf (p, fmt, ap);

  if (p >= buf+1024) abort ();

  // pthr_print_report (stderr, buf, 1);
}


void
qGlobeError (char const *prefix, char const *fmt, ...)
{
  va_list ap;
  va_start (ap, fmt);
  pthr_verror1 (prefix, fmt, ap);
  va_end (ap);
}

void * 
qGlobeMemMalloc(size_t size)
{
 static char * funcName = "qGlobeMemMalloc()";
 void * buffer;

    buffer = malloc(size);
    if(!buffer) {
      qGlobeError (funcName, "Error in malloc.");
      return NULL;
    }
    return buffer;
}

void * 
qGlobeMemRealloc(void * ptr, 
		     size_t size)
{
  static char * funcName = "qGlobeMemRealloc()";
  void * buffer;

  buffer = realloc(ptr, size);
  
  if(!buffer && size) {
    qGlobeError (funcName, "Error in realloc'ing %d bytes.", size);
    return (void *) -1;
  }
  return buffer;
}

void * 
qGlobeMemCalloc(size_t nelem,
		    size_t elsize)
{
 static char * funcName = "qGlobeMemCalloc()";
 void * buffer;

    buffer = calloc(nelem, elsize);
    if(!buffer) {
      qGlobeError (funcName, "Error in callocing %d items of size %d.", nelem, elsize);
      return NULL;
    }
    return buffer;
}

int
qGlobeMemFree(void const * ptr)
{
  /*static const char funcName[] = "qGlobeMemFree()";*/

  free((void *) ptr);
  return TRUE;
}

char *
qGlobeMemStringCpy(const char * s1)
{
 static char * funcName = "qGlobeMemStringCpy()";
 char * s2;
 size_t len;

    if(!s1) {
      qGlobeError (funcName, "NULL passed to strdup, idiot."); 
      return NULL;
    }
    len = strlen(s1) + 1;
    s2 = (char*)qGlobeMemMalloc(len);
    if(!s2) {
      qGlobeError (funcName, "Unable to dup string of len %zu: %s", len - 1, s1);
      return NULL;
    }
    memcpy (s2, s1, len);
    return s2;
}
