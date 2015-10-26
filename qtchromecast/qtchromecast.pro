#-------------------------------------------------
#
# Project created by QtCreator 2015-10-27T01:01:06
#
#-------------------------------------------------

QT       += core network gui


TARGET = qtchromecast
TEMPLATE = app

DEFINES += QTCHROMECAST_LIBRARY

INCLUDEPATH += ./core \
./protocols/ssdp

LIBS += -lWs2_32

SOURCES += qtchromecast.cpp \
    protocols/ssdp/ssdpclient.cpp \
    core/udpmulticastlistener.cpp \
    ssdpclienttest.cpp

HEADERS += qtchromecast.h\
        qtchromecast_global.h \
    protocols/ssdp/ssdpclient.h \
    core/udpmulticastlistener.h \
    ssdpclienttest.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
