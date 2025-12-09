#pragma once 
#include <Arduino.h>
#include "types.h"


extern TaskHandle_t runPIDTaskHandle;
extern TaskHandle_t runPWMTaskHandle;

void PIDControl();
void PWMControl();

void startRunPIDTask(uint8_t core);
void startRunPWMTask(uint8_t core);