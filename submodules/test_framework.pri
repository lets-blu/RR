isEmpty(TEST_FRAMEWORK_DIR):TEST_FRAMEWORK_DIR=$$PWD/../test_framework

!isEmpty(TEST_FRAMEWORK_DIR): {
    GTEST_DIR = $$TEST_FRAMEWORK_DIR/googletest
    GMOCK_DIR = $$TEST_FRAMEWORK_DIR/googlemock
}

requires(exists($$GTEST_DIR):exists($$GMOCK_DIR))

INCLUDEPATH *= \
    $$GTEST_DIR \
    $$GTEST_DIR/include \
    $$GMOCK_DIR \
    $$GMOCK_DIR/include

SOURCES += \
    $$GTEST_DIR/src/gtest-all.cc \
    $$GMOCK_DIR/src/gmock-all.cc
