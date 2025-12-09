#include "tasks/testPWM.h"

void testPWMTask(void* pvParameters) {
    Serial.println("Tarea de prueba del PWM iniciada.");
    // Aquí iría la lógica específica para la prueba del PWM

    vTaskDelete(NULL); // Eliminar la tarea al finalizar
}