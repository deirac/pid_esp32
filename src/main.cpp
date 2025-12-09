#include <Arduino.h>
#include "menuTask.h"
#include "controlTask.h"
#include "types.h"


void setup() {
    Serial.begin(115200);
    delay(1000); // Esperar a que la conexión serial se estabilice
    Serial.println(F("Iniciando sistema..."));
    
    startMenuTask();
    startControlTask(); 
}

void loop() {

}