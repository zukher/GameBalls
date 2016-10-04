QT += core
QT -= gui

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

SOURCES += main.cpp \
    Board.cpp \
    Cell.cpp \
    GamePlay.cpp

HEADERS += \
    Board.h \
    Cell.h \
    GamePlay.h
