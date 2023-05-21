#include <RR.h>
#include <port/arduino/inc/ArduinoFactory.h>

ArduinoFactory factory;
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
  DeviceManager *manager = instanceOfDeviceManager();

  Serial.begin(115200);
#if defined(ARDUINO_ARCH_AVR)
  fdevopen(serialPutc, NULL);
#endif

  constructArduinoFactory(&factory);
  setFactoryToDeviceManager(manager, &factory.base);
  setBasePinToDeviceManager(manager, 1, sizeof(ArduinoPin));

  constructLED(&led, NULL, LED_BUILTIN, PIN_STATE_HIGH);
}

void loop() {
  turnOnLED(&led);
  delay(1000);

  turnOffLED(&led);
  delay(1000);
}

