#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "GDM_Define.h"
#include "GDM_Mem.h"

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
gdmError (char const *prefix, char const *fmt, ...)
{
  va_list ap;
  va_start (ap, fmt);
  pthr_verror1 (prefix, fmt, ap);
  va_end (ap);
}

void * 
gdmMemMalloc(size_t size)
{
 static char * funcName = "gdmMemMalloc()";
 void * buffer;

    buffer = malloc(size);
    if(!buffer) {
      gdmError (funcName, "Error in malloc.");
      return NULL;
    }
    return buffer;
}

void * 
gdmMemRealloc(void * ptr, 
		     size_t size)
{
  static char * funcName = "gdmMemMalloc()";
  void * buffer;

  buffer = realloc(ptr, size);
  
  if(!buffer && size) {
    gdmError (funcName, "Error in realloc'ing %d bytes.", size);
    return (void *) -1;
  }
  return buffer;
}

void * 
gdmMemCalloc(size_t nelem,
		    size_t elsize)
{
 static char * funcName = "gdmMemCalloc()";
 void * buffer;

    buffer = calloc(nelem, elsize);
    if(!buffer) {
      gdmError (funcName, "Error in callocing %d items of size %d.", nelem, elsize);
      return NULL;
    }
    return buffer;
}

int
gdmMemFree(void const * ptr)
{
  /*static const char funcName[] = "gdmMemManagerFree()";*/

  free((void *) ptr);
  return TRUE;
}

char *
gdmMemStringCpy(const char * s1)
{
 static char * funcName = "gdmMemStringCpy()";
 char * s2;
 int len;

    if(!s1) {
      gdmError (funcName, "NULL passed to strdup, idiot."); 
      return NULL;
    }
    len = strlen(s1) + 1;
    s2 = (char*)gdmMemMalloc(len);
    if(!s2) {
      gdmError (funcName, "Unable to dup string of len %d: %s", strlen(s1), s1);
      return NULL;
    }
    memcpy (s2, s1, len);
    return s2;
}
