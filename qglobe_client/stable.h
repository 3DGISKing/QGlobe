#ifndef STABLE_H
#define STABLE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <iostream>
#include <vector>
#include <QtCore/QObject>
#include <QtWidgets/QApplication>
#include <QtWidgets/QOpenGLWidget>
#include <QtOpenGL/QtOpenGL>
#include <QtCore/QTranslator>
#include <QtCore/QVariant>

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#define qVariantValue qvariant_cast

// Qt4 compatibility shim to keep legacy QApplication::UnicodeUTF8 calls
// compiling without touching generated ui_ headers.
class QApplicationCompat : public QApplication
{
public:
    using QApplication::QApplication;
    enum { UnicodeUTF8 = -1 };
};

#define QApplication QApplicationCompat
#endif

#endif // STABLE_H
