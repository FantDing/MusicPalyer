#-------------------------------------------------
#
# Project created by QtCreator 2015-08-06T20:15:05
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MJ
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    search.cpp

HEADERS  += widget.h \
    search.h

FORMS    += widget.ui \
    search.ui

RESOURCES += \
    res.qrc

DISTFILES += \
    myapp.rc

RC_FILE = myapp.rc
