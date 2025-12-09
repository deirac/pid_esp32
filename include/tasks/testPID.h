#pragma once
#include <Arduino.h>
#include "types.h"


extern TaskHandle_t testPIDTaskHandle;
extern TaskHandle_t testPIDControlTaskHandle;
extern volatile SystemMode currentMode;

void testPID();
void testPIDControl();

void testPIDTask(uint8_t core);
void testPIDControlerTask(uint8_t core);