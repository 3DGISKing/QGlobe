#ifndef GDEM_GIS_H
#define GDEM_GIS_H

#include <qglobal.h>

#ifdef GDEM_STATIC_LIBRARY
# define GDEM_GIS_EXPORT Q_DECL_IMPORT
#else
# define GDEM_GIS_EXPORT Q_DECL_EXPORT
#endif

#endif // GDEM_GIS_H
