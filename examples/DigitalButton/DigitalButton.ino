#include <RR.h>
#include <port/arduino/inc/ArduinoFactory.h>

ArduinoFactory factory;
DigitalButton button;
EventHandler pushHandler;
EventHandler releaseHandler;

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
  constructEventHandler(&pushHandler, onPush);
  constructEventHandler(&releaseHandler, onRelease);

  addPushHandlerToDigitalButton(&button, &pushHandler);
  addReleaseHandlerToDigitalButton(&button, &releaseHandler);
}

void loop() {
  scanDigitalButton(&button);
  delay(50);
}

void onPush(EventHandler *handler, void *sender, void *argument) {
  (void)argument;

  if (handler == &pushHandler && sender == &button) {
    LOG_I(&filter, "Button pushed!");
  }
}

void onRelease(EventHandler *handler, void *sender, void *argument) {
  (void)argument;

  if (handler == &releaseHandler && sender == &button) {
    LOG_I(&filter, "Button released!");
  }
}

