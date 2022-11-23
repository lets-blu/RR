#include <RR.h>

#if defined(ARDUINO_ARCH_AVR)
#include <Arduino_FreeRTOS.h>
#elif defined(ARDUINO_ARCH_STM32)
#include <STM32FreeRTOS.h>
#endif

MutexResource resource;
LogFilter filter = STATIC_LOG_FILTER("main", LOG_LEVEL_INFO);

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

void setupSyslog() {
  LogPrinterManager *manager = instanceOfLogPrinterManager();
  StandardLogPrinter *printer = instanceOfStandardLogPrinter();

  Serial.begin(115200);
#if defined(ARDUINO_ARCH_AVR)
  fdevopen(serialPutc, NULL);
#endif

  constructMutexResource(&resource);
  setResourceToStandardLogPrinter(printer, &resource.resource);

  addPrinterToLogPrinterManager(manager, "default", &printer->printer);
  setDefaultPrinterToLogPrinterManager(manager, "default");
}

void blink(void *arg) {
  (void)arg;

  LED led = {0};
  GPIOPin ledPin = {0};

  constructGPIOPin(&ledPin, GENERIC_ARDUINO_PORT, LED_BUILTIN);
  constructLED(&led, &ledPin, HIGH);
  deconstructGPIOPin(&ledPin);

  for (;;) {
    turnOnLED(&led);
    vTaskDelay(1000);

    turnOffLED(&led);
    vTaskDelay(1000);
  }
}

void setup() {
  setupSyslog();

  if (xTaskCreate(blink, NULL, 256, NULL, 1, NULL) != pdPASS) {
    LOG_E(&filter, "Cannot create task");
    return;
  }

  vTaskStartScheduler();
}

void loop() {
  // Never used
}
