#include <RR.h>
#include <port/arduino/inc/ArduinoFactory.h>

#define REGISTERS_NUM 2

ArduinoFactory factory;
ShiftRegister shiftRegisters[REGISTERS_NUM];
RegisterGroup group;

#if defined(ARDUINO_ARCH_AVR)
int serialPutc(char c, FILE *file) {
  (void)file;
  Serial.write(c);
  return c;
}
#elif defined(ARDUINO_ARCH_STM32)
int fputc(int c, FILE *file) {
  (void)file;
  Serial.write(c);
  return c;
}
#endif

void setup() {
  RegisterGroupParameter parameter = {
    .oePort   = NULL,
    .oePin    = 2,
    .serPort  = NULL,
    .serPin   = 3,
    .sckPort  = NULL,
    .sckPin   = 4,
    .rckPort  = NULL,
    .rckPin   = 5
  };

  DeviceManager *manager = instanceOfDeviceManager();

  Serial.begin(115200);
#if defined(ARDUINO_ARCH_AVR)
  fdevopen(serialPutc, NULL);
#endif

  constructArduinoFactory(&factory);
  setFactoryToDeviceManager(manager, &factory.base);
  setBasePinToDeviceManager(manager, 4, sizeof(ArduinoPin));

  constructRegisterGroup(&group, &parameter);

  for (unsigned int i = 0; i < REGISTERS_NUM; i++) {
    constructShiftRegister(&shiftRegisters[i]);
    addRegisterToRegisterGroup(&group, &shiftRegisters[i]);
  }

  outputRegisterGroup(&group);
  setOutputEnableToRegisterGroup(&group, true);
}

void loop() {
  for (unsigned int i = 0; i < REGISTERS_NUM; i++) {
    ShiftRegister *shiftRegister = &shiftRegisters[i];

    for (unsigned int j = 0; j < 8; j++) {
      if (j > 0) {
        resetBitFromShiftRegister(shiftRegister, j - 1);
      }

      setBitToShiftRegister(shiftRegister, j);
      outputRegisterGroup(&group);
      delay(50);
    }

    // reset last bit for next register
    resetBitFromShiftRegister(shiftRegister, 7);
  }
}

