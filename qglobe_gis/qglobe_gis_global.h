#ifndef QGLOBE_GIS_H
#define QGLOBE_GIS_H

#include <qglobal.h>

#ifdef QGLOBE_STATIC_LIBRARY
# define QGLOBE_GIS_EXPORT Q_DECL_IMPORT
#else
# define QGLOBE_GIS_EXPORT Q_DECL_EXPORT
#endif

#endif // QGLOBE_GIS_H
