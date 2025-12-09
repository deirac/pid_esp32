#pragma once
#include <Arduino.h>
#include "types.h"

void initPWM(PWMData* pwm);
void updatePWMData(PWMData* pwm);
void setPWMDuty(float duty_percent);
void printPWMData(PWMData* pwm);

void rampPWMDuty(float start_duty, float end_duty, uint32_t ramp_time_ms);

