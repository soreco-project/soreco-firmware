#include "SystemInitializeDriver.h"

void SystemInitializeDriver::initPinConfig(void) {
    // GPIO 2 as an output -> LED
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
}