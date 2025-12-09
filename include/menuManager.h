#pragma once
#include <Arduino.h>
#include "types.h"

// ---------------------------------------------------------
// Declaraciones de funciones para la gestión del sistema   
// ---------------------------------------------------------
void printMenu();
SystemMode handleUserInput();
const char* getModeName(SystemMode mode);

