#ifndef GDEM_BASE_GLOBAL_H
#define GDEM_BASE_GLOBAL_H

#include <qglobal.h>

#ifdef GDEM_STATIC_LIBRARY
# define GDEM_BASE_EXPORT   Q_DECL_IMPORT
#else
# define GDEM_BASE_EXPORT Q_DECL_EXPORT
#endif

#endif // GDEM_BASE_GLOBAL_H
