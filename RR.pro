TEMPLATE = app
CONFIG += console c++11 thread
CONFIG -= app_bundle qt

# Add RR
RR_DIR = $$PWD/src
requires(exists($$RR_DIR))

INCLUDEPATH *= \
    $$RR_DIR

SOURCES += \
    $$RR_DIR/src/Utility/LinkedList.c \
    $$RR_DIR/src/Utility/ArrayList.c

# Add test case
CASE_DIR = $$PWD/test_case
requires(exists($$CASE_DIR))

HEADERS += \
    $$CASE_DIR/LinkedListTest.h \
    $$CASE_DIR/ArrayListTest.h

SOURCES += \
    $$CASE_DIR/main.cpp             \
    $$CASE_DIR/LinkedListTest.cpp   \
    $$CASE_DIR/ArrayListTest.cpp

# Add test framework
GTEST_DIR = $$PWD/test_framework/googletest
GMOCK_DIR = $$PWD/test_framework/googlemock
requires(exists($$GTEST_DIR):exists($$GMOCK_DIR))

INCLUDEPATH *= \
    $$GTEST_DIR         \
    $$GTEST_DIR/include \
    $$GMOCK_DIR         \
    $$GMOCK_DIR/include

SOURCES += \
    $$GTEST_DIR/src/gtest-all.cc \
    $$GMOCK_DIR/src/gmock-all.cc
