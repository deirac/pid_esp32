#include "tasks/testEncoder.h"
#include "drivers/encoder.h"
#include "config.h"


TaskHandle_t testEncoderTaskHandle = nullptr;
TaskHandle_t testEncoderTaskControlHandle = nullptr;


// =======================================================
// Tarea principal de prueba del encoder
// =======================================================
void testEncoder() {
     
    Serial.println("[TEST_ENCODER] Tarea iniciada");

    initEncoder();
    vTaskDelay(pdTICKS_TO_MS(100)); // Esperar a que el encoder se estabilice
    EncoderData data;

    while (true) {
         data = getEncoderData();
        printEncoderData(data);
        Serial.println("-----------------------------");
        vTaskDelay(pdMS_TO_TICKS(data.sampling_time_ms));
    }
}

void testEncoderTask(uint8_t core) {
    if(testEncoderTaskHandle == NULL) {
        xTaskCreatePinnedToCore(
            [](void* pvParameters) {
                testEncoder();
            },
            "TestEncoderTask",        // Text name for the task.
            2048,                     // Stack size in words, not bytes.
            NULL,                     // Parameter passed into the task.
            2,                        // Priority at which the task is created.
            &testEncoderTaskHandle, 
            core
        );  // Used to pass out the created task's handle.
    }
}

// =======================================================
// Tarea de control para detener la prueba del encoder  
// =======================================================

void testEncoderControl() {

    Serial.println("[TEST_ENCODER_CONTROL] Tarea iniciada");
    Serial.println("Presione 'c' para cancelar la prueba del encoder.");

    while (true) {

        if (Serial.available() > 0) {
            char input = Serial.read();

            if (input == 'c') {
                Serial.println("[TEST_ENCODER_CONTROL] Cancelando prueba de encoder...");

                // --- Detener la tarea principal del encoder ---
                if(testEncoderTaskHandle != NULL) {
                    vTaskDelete(testEncoderTaskHandle);
                    testEncoderTaskHandle = NULL;
                }
                
                currentMode = MODE_IDLE;

                Serial.println("[TEST_ENCODER_CONTROL] Prueba cancelada.");

                // --- Eliminar sólo esta tarea controladora ---
                if (testEncoderTaskControlHandle != NULL) {
                    vTaskDelete(testEncoderTaskControlHandle);
                    testEncoderTaskControlHandle = NULL;
                }
                
            }
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void testEncoderControlTask(uint8_t core) {
    if(testEncoderTaskControlHandle == NULL) {
        xTaskCreatePinnedToCore(
            [](void* pvParameters) {
                testEncoderControl();
            },
            "TestEncoderControlTask", // Text name for the task.
            2048,                     // Stack size in words, not bytes.
            NULL,                     // Parameter passed into the task.
            2,                        // Priority at which the task is created.
            &testEncoderTaskControlHandle, 
            core
        );  // Used to pass out the created task's handle.
    }
}