include(submodules/hal.pri)
include(submodules/test_framework.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += thread

INCLUDEPATH *= \
    inc

QMAKE_CFLAGS += \
    -DUNIT_TEST

QMAKE_CXXFLAGS += \
    -DUNIT_TEST

SOURCES += \
    src/gpiopin.c \
    test_cases/main.cpp \
    test_cases/test_gpiopin.cpp
