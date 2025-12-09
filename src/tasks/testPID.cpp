#include "tasks/testPID.h"

void testPIDTask(void* pvParameters) {
    Serial.println("Tarea de prueba del PID iniciada.");
    // Aquí iría la lógica específica para la prueba del PID

    vTaskDelete(NULL); // Eliminar la tarea al finalizar
}