#-------------------------------------------------
#
# Project created by QtCreator 2018-08-12T11:19:38
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Word
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Settings/s.cpp \
    Settings/settings_gui.cpp

HEADERS  += mainwindow.h \
    Settings/s.h \
    Settings/settings_gui.h \
    include.h
