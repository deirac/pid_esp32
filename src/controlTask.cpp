#include "controlTask.h"
#include "menuManager.h"
#include "menuTask.h"
#include "subTask.h"

static void controlTask(void* pvParameters) {
    Serial.println("ControlTask iniciado");
    vTaskDelay(pdMS_TO_TICKS(500));
    
    while(currentMode==SystemMode::MODE_IDLE) {// Esperar hasta que el modo cambie desde IDLE
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    vTaskDelay(pdMS_TO_TICKS(2000));
    Serial.println("Seleción finalizada.");

    Serial.println("Modo confirmado: ");
    Serial.println(getModeName(currentMode));
    vTaskDelay(pdMS_TO_TICKS(500));

    switch (currentMode) { // Seleccionar la tarea según el modo actual
    case MODE_TEST_ENCODER:     startTestEncoderTask();     break;
    case MODE_TEST_PWM:         startTestPWMTask();         break;
    case MODE_TEST_PID:         startTestPIDTask();         break;
    case MODE_CALIBRATION:      startCalibrateTask();       break;
    case MODE_FORWARD:          startRunMotorTask();        break;
    default:
        Serial.println("Modo desconocido. Terminando tarea de control.");
        break;
    }
    while (currentMode != MODE_IDLE) {
            vTaskDelay(pdMS_TO_TICKS(100));
        }

        // Regresar al menú
        Serial.println("Regresando al menú...");
        startMenuTask();
}

void startControlTask() {
    xTaskCreatePinnedToCore(
        controlTask,          
        "Control Task",      
        8192,                
        NULL,                
        1,                   
        NULL,                
        0                    
    );
}