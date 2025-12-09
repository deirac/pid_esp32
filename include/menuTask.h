#pragma once
#include <Arduino.h>
#include "types.h"

void menuTask(void* parameter);
void startMenuTask();
void deleteMenuTask();
extern volatile SystemMode currentMode;