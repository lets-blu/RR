#include <RR.h>
#include <port/arduino/inc/ArduinoFactory.h>

ArduinoFactory factory;
DigitalButton button;
EventHandler handler;

LogFilter filter = STATIC_LOG_FILTER("Main", LOG_LEVEL_INFO);

#if defined(ARDUINO_ARCH_AVR)
int serialPutc(char c, FILE *file) {
  (void)file;
  Serial.write(c);
  return c;
}
#elif defined(ARDUINO_ARCH_STM32)
int fputc(char c, FILE *file) {
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

  constructDigitalButton(&button, NULL, A5, PIN_STATE_LOW);
  constructEventHandler(&handler, onClick);
  addClickHandlerToDigitalButton(&button, &handler);
}

void loop() {
  scanDigitalButton(&button);
  delay(50);
}

void onClick(EventHandler *handler, void *sender, void *argument) {
  (void)handler;
  (void)argument;

  if (sender == &button) {
    LOG_I(&filter, "Button clicked!");
  } else {
    LOG_I(&filter, "Unknown sender 0x%x", sender);
  }
}

