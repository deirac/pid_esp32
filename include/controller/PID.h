#pragma once
#include <Arduino.h>
#include "types.h"

void initPID(PIDData* pid);
float computePID(PIDData* pid, float measurement, float dt);

void resetPID(PIDData* pid);