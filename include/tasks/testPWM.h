#pragma once
#include <Arduino.h>
#include "types.h"

extern TaskHandle_t testPWMTaskHandle;
extern TaskHandle_t testPWMControlerTaskHandle;
extern volatile SystemMode currentMode;

void testPWM();
void testPWMControler();


void testPWMTask(uint8_t core);
void testPWMControlerTask(uint8_t core);