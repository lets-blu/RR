#include <RR.h>
#include <port/arduino/inc/ArduinoFactory.h>

ArduinoFactory factory;
DevicePool pool;

DataSelector selector;
DataSelectorHandler handler;
DataSelectorGroup group;

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
  DataSelectorGroupParameter parameter= {
    .dataPin = {NULL, A5},
    .addressPins = {&PORTC, (1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC4)}
  };

  DeviceManager *manager = instanceOfDeviceManager();

  Serial.begin(115200);
#if defined(ARDUINO_ARCH_AVR)
  fdevopen(serialPutc, NULL);
#endif

  constructArduinoFactory(&factory);
  setFactoryToDeviceManager(manager, &factory.base);

  constructDevicePool(&pool, 2, getPinSizeFromBaseFactory(&factory.base));
  setPinPoolToDeviceManager(manager, &pool);

  constructDataSelector(&selector, 0, 4);
  constructDataSelectorHandler(&handler, 0, onDataChange);
  constructDataSelectorGroup(&group, &parameter);

  addChangeHandlerToDataSelector(&selector, &handler);
  addSelectorToDataSelectorGroup(&group, &selector);
  scanDataSelectorGroup(&group);
}

void loop() {
  scanDataSelectorGroup(&group);
  delay(500);
}

void onDataChange(EventHandler *handler, void *sender, void *parameter)
{
  (void)handler;
  (void)sender;
  (void)parameter;

  LOG_I(&filter, "onDataChange!");
}

