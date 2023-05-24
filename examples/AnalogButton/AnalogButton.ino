#include <RR.h>
#include <port/arduino/inc/ArduinoFactory.h>

#define BUTTON_NUM 5

ArduinoFactory factory;
AnalogButtonGroup group;
AnalogButton button[BUTTON_NUM];
EventHandler handler[BUTTON_NUM];

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

  constructAnalogButton(&button[0], 0, 100);
  constructAnalogButton(&button[1], 101, 300);
  constructAnalogButton(&button[2], 301, 500);
  constructAnalogButton(&button[3], 501, 700);
  constructAnalogButton(&button[4], 701, 900);
  constructAnalogButtonGroup(&group, NULL, A0);

  for (unsigned int i = 0; i < BUTTON_NUM; i++) {
      constructEventHandler(&handler[i], onClick);
      addClickHandlerToAnalogButton(&button[i], &handler[i]);
      addButtonToAnalogButtonGroup(&group, &button[i]);
  }
}

void loop() {
  scanAnalogButtonGroup(&group);
  delay(50);
}

void onClick(EventHandler *handler, void *sender, void *argument) {
  (void)handler;
  (void)sender;

  if (sender == &button[0]) {
    LOG_I(&filter, "Button 0 clicked!");
  } else if (sender == &button[1]) {
    LOG_I(&filter, "Button 1 clicked!");
  } else if (sender == &button[2]) {
    LOG_I(&filter, "Button 2 clicked!");
  } else if (sender == &button[3]) {
    LOG_I(&filter, "Button 3 clicked!");
  } else if (sender == &button[4]) {
    LOG_I(&filter, "Button 4 clicked!");
  } else {
    LOG_I(&filter, "Unknown sender 0x%x", sender);
  }
}

