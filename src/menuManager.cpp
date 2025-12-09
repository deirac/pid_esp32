#include "menuManager.h"



void printMenu() { // Función para imprimir el menú de opciones
    Serial.println(F("===== Menú de Control del Sistema ====="));
    Serial.println(F("1: Modo Idle"));
    Serial.println(F("2: Prueba del Encoder"));
    Serial.println(F("3: Prueba del PWM"));
    Serial.println(F("4: Prueba del Controlador PID"));
    Serial.println(F("5: Calibración"));
    Serial.println(F("6: Operación Normal"));
    Serial.println(F("========================================"));
    Serial.print(F("Seleccione una opción: >>> "));
}


SystemMode handleUserInput(){
    while (Serial.available() == 0) {// Esperar hasta que haya datos disponibles
        vTaskDelay(pdTICKS_TO_MS(10));
    }
    char input = Serial.read();     // Leer el carácter ingresado
    Serial.println(input);          // Eco del carácter ingresado

    switch (input) {
        case '1': return MODE_IDLE;
        case '2': return MODE_TEST_ENCODER;
        case '3': return MODE_TEST_PWM;
        case '4': return MODE_TEST_PID;
        case '5': return MODE_CALIBRATION;
        case '6': return MODE_FORWARD;
        default:
            Serial.println(F("Opción inválida."));
            return MODE_IDLE;
    }
}

const char* getModeName(SystemMode mode) {
    switch (mode) {
        case MODE_IDLE:             return "Idle";
        case MODE_TEST_ENCODER:     return "Prueba del Encoder";
        case MODE_TEST_PWM:         return "Prueba del PWM";
        case MODE_TEST_PID:          return "Prueba del Controlador PID";
        case MODE_CALIBRATION:      return "Calibración";
        case MODE_FORWARD:          return "Operación Normal";
        default:                    return "Desconocido";
    }
}