#include "tasks/calibrate.h"

void calibrateTask(void* pvParameters) {
    Serial.println("Tarea de calibración iniciada.");
    // Aquí iría la lógica específica para la calibración

    vTaskDelete(NULL); // Eliminar la tarea al finalizar
}