#pragma once
#include <Arduino.h>
#include "types.h"

static void IRAM_ATTR encoderISR();
void initEncoder();
EncoderData getEncoderData();
void printEncoderData(const EncoderData& data);
