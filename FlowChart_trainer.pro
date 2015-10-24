#-------------------------------------------------
#
# Project created by QtCreator 2015-06-19T08:50:37
#
#-------------------------------------------------

QT       += core gui xml
win32: CONFIG += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FlowChart_trainer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    fc_widget.cpp \
    inputdialog.cpp \
    testwindow.cpp \
    exercisewindow.cpp

HEADERS  += mainwindow.h \
    fc_widget.h \
    inputdialog.h \
    testwindow.h \
    exercisewindow.h

FORMS    += mainwindow.ui \
    testwindow.ui \
    exercisewindow.ui

RESOURCES += \
    fc_resources.qrc
