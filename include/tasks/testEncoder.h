#pragma once
#include <Arduino.h>
#include "types.h"

extern TaskHandle_t testEncoderTaskHandle;
extern TaskHandle_t testEncoderTaskControlHandle;
extern volatile SystemMode currentMode;

void testEncoder();
void testEncoderControl();

void testEncoderTask(uint8_t core);
void testEncoderControlTask(uint8_t core);
