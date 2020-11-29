include(submodules/hal.pri)
include(submodules/test_framework.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += thread

INCLUDEPATH *= \
    inc \
    inc/generic \
    inc/digital \
    inc/interface

QMAKE_CFLAGS += \
    -DUNIT_TEST \
    -fprofile-arcs \
    -ftest-coverage

QMAKE_CXXFLAGS += \
    -DUNIT_TEST

QMAKE_LFLAGS += \
    -fprofile-arcs \
    -ftest-coverage

SOURCES += \
    src/gpiopin.c \
    src/generic/led.c \
    src/generic/button.c \
    src/interface/chainedobserver.c \
    src/interface/messagequeue.c \
    src/digital/shiftregister.c \
    src/digital/registergroup.c \
    src/digital/dataselector.c \
    test_cases/main.cpp \
    test_cases/test_gpiopin.cpp \
    test_cases/test_led.cpp \
    test_cases/test_button.cpp \
    test_cases/test_shiftregister.cpp \
    test_cases/test_registergroup.cpp \
    test_cases/test_dataselector.cpp
