#pragma once
#include <Arduino.h>
#include "types.h"

void initPWM();
PWMData getPWMData();
void setPWMDuty(float duty_percent);
void printPWMData();

void rampPWMDuty(float start_duty, float end_duty, uint32_t ramp_time_ms);

