#include <RR.h>
#include <port/arduino/inc/ArduinoFactory.h>

ArduinoFactory factory;
DevicePool pool;
LED led;

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
  BasePinParameter parameter = {
    .port = NULL,
    .pin  = LED_BUILTIN
  };

  DeviceManager *manager = instanceOfDeviceManager();

  Serial.begin(115200);
#if defined(ARDUINO_ARCH_AVR)
  fdevopen(serialPutc, NULL);
#endif

  constructArduinoFactory(&factory);
  setFactoryToDeviceManager(manager, &factory.base);

  constructDevicePool(&pool, 1, getPinSizeFromBaseFactory(&factory.base));
  setPinPoolToDeviceManager(manager, &pool);

  constructLED(&led, &parameter, BASE_PIN_VALUE_HIGH);
}

void loop() {
  turnOnLED(&led);
  delay(1000);

  turnOffLED(&led);
  delay(1000);
}

