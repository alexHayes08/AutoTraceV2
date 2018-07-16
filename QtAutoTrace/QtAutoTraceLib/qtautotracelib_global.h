#pragma once

#include <QtCore/qglobal.h>

#if defined(QTAUTOTRACELIB_LIBRARY)
#  define QTAUTOTRACELIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTAUTOTRACELIBSHARED_EXPORT Q_DECL_IMPORT
#endif
