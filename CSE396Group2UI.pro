#-------------------------------------------------
#
# Project created by QtCreator 2017-04-10T17:17:42
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = CSE396Group2UI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    scene2d.cpp \
    connectionthread.cpp \
    coordinatebrowserth.cpp \
    qcustomplot.cpp \
    scene2dth.cpp

HEADERS  += mainwindow.h \
    scene2d.h \
    connectionthread.h \
    coordinatebrowserth.h \
    qcustomplot.h \
    scene2dth.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=gnu++14

RESOURCES += \
    images.qrc
