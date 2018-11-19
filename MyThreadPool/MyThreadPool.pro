TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    threadpool.cpp \
    threadtask.cpp \
    threadwork.cpp

HEADERS += \
    threadpool.h \
    threadtask.h \
    threadwork.h \
    def.h
