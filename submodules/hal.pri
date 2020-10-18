isEmpty(HAL_DIR):HAL_DIR = $$PWD/../hal

requires(exists($$HAL_DIR))

INCLUDEPATH *= \
    $$HAL_DIR/inc

SOURCES += \
    $$HAL_DIR/src/hal_gpio.c
