QT += core network
QT -= gui

CONFIG += c++11

TARGET = yu_test
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    tcpserver.cpp \
    sysvar.cpp \
    tcpclient.cpp \
    udpserver.cpp \
    savedatathread.cpp \
    udpclient.cpp

HEADERS += \
    tcpserver.h \
    sysvar.h \
    tcpclient.h \
    udpserver.h \
    savedatathread.h \
    udpclient.h
