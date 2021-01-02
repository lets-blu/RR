isEmpty(HAL_DIR):HAL_DIR = $$PWD/../hal

requires(exists($$HAL_DIR))

INCLUDEPATH *= \
    $$HAL_DIR/inc

SOURCES += \
    $$HAL_DIR/src/cmsis_os.c \
    $$HAL_DIR/src/hal_gpio.c \
    $$HAL_DIR/src/hal_cortex.c \
    $$HAL_DIR/src/hal_iwdg.c
