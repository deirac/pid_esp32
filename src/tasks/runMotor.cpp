#include "tasks/runMotor.h"

void runMotorTask(void* pvParameters) {
    Serial.println("Tarea de ejecución del motor iniciada.");
    // Aquí iría la lógica específica para ejecutar el motor

    vTaskDelete(NULL); // Eliminar la tarea al finalizar
}
 