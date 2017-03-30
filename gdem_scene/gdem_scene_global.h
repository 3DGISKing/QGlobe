#ifndef GDEM_SCENE_GLOBAL_H
#define GDEM_SCENE_GLOBAL_H

#include <qglobal.h>

#ifdef GDEM_STATIC_LIBRARY
# define GDEM_SCENE_EXPORT Q_DECL_IMPORT
#else
# define GDEM_SCENE_EXPORT Q_DECL_EXPORT
#endif

#endif // GDEM_SCENE_GLOBAL_H
