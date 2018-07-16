#-------------------------------------------------
#
# Project created by QtCreator 2018-07-15T14:16:58
#
#-------------------------------------------------

QT       += svg

QT       -= gui

TARGET = QtAutoTraceLib
TEMPLATE = lib

DEFINES += QTAUTOTRACELIB_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        adjacentpixels.cpp \
        generictinputreader.cpp \
        inputoptions.cpp \
        inputparser.cpp \
        pixelgroup.cpp \
        outputoptions.cpp \
        qtautotracelib.cpp \
        splinelistarray.cpp \
        svgelementfactory.cpp \
        svgelement.cpp

HEADERS += \
        qtautotracelib.h \
        qtautotracelib_global.h \
        adjacentpixels.h \
        filenotfoundexception.h \
        filereadexception.h \
        generictinputreader.h \
        inputoptions.h \
        inputparser.h \
        notimplementedexception.h \
        pixeldata.h \
        outputoptions.h \
        pixelgroup.h \
        qtautotracelib_global.h \
        qtautotracelib.h \
        splinelistarray.h \
        svgelementfactory.h \
        svgelement.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
