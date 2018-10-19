TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    threadtask.cpp \
    mythread.cpp \
    workthread.cpp \
    threadpool.cpp

HEADERS += \
    threadtask.h \
    mythread.h \
    workthread.h \
    threadpool.h
