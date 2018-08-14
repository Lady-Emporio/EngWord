#-------------------------------------------------
#
# Project created by QtCreator 2018-08-13T14:48:07
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EngWord
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Settings/s.cpp \
    Settings/settings_gui.cpp \
    Forms/formobject.cpp \
    Forms/dynamiclist.cpp \
    Forms/mdi_dl.cpp

HEADERS  += mainwindow.h \
    include.h \
    Settings/s.h \
    Settings/settings_gui.h \
    separateclearfunc.h \
    Forms/formobject.h \
    Forms/dynamiclist.h \
    Forms/mdi_dl.h
