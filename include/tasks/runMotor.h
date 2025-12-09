#pragma once
#include <Arduino.h>

extern TaskHandle_t runPIDTaskHandle;


void runMotorTask(void* pvParameters);
void startRunPIDTask(uint8_t core);