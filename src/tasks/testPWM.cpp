#include "tasks/testPWM.h"
#include "drivers/pwm.h"
#include "config.h"

TaskHandle_t testPWMTaskHandle = nullptr;
TaskHandle_t testPWMControlerTaskHandle = nullptr;


// =======================================================
// Tarea principal de prueba del PWM
// =======================================================
///@brief Función que realiza la prueba del PWM
///@param void
///@return void
void testPWM() {
    Serial.println("[TEST_PWM] Tarea iniciada");
    
    initPWM();
    vTaskDelay(pdTICKS_TO_MS(100)); // Esperar a que el PWM se estabilice
    float top_duty = 0.0f;

    while (true) {
        top_duty += 0.05f;
        if (top_duty > DUTY_MAX) {
            top_duty = DUTY_MIN;
        }
        rampPWMDuty(0.0f, top_duty, 100); // Rampa hacia arriba
        PWMData data = getPWMData();
        printPWMData();
        Serial.println("-----------------------------");
        vTaskDelay(pdMS_TO_TICKS(data.sampling_time_ms));
    }
}

/// @brief  Tarea para ejecutar la prueba del PWM en un núcleo específico
/// @param core 
/// @return void
void testPWMTask(uint8_t core) {
   if (testPWMTaskHandle == NULL) {
        xTaskCreatePinnedToCore(
            [](void* pvParameters) {
                testPWM();
            },
            "TestPWMTask",        // Text name for the task.
            2048,                 // Stack size in words, not bytes.
            NULL,                 // Parameter passed into the task.
            2,                    // Priority at which the task is created.
            &testPWMTaskHandle, 
            core
        );  // Used to pass out the created task's handle.
    }
    
}

// =======================================================
// Tarea de control para la prueba del PWM
// =======================================================
/// @brief Función que controla la prueba del PWM
/// @param void
/// @return void
void testPWMControler() {
    Serial.println("[TEST_PWM_CONTROL] Tarea iniciada");
    Serial.println("Presione 'c' para cancelar la prueba del PWM.");

    while (true) {

        if (Serial.available() > 0) {
            char input = Serial.read();

            if (input == 'c') {
                Serial.println("[TEST_PWM_CONTROL] Cancelando prueba de PWM...");

                if (testPWMTaskHandle != NULL) {
                    vTaskDelete(testPWMTaskHandle);
                    testPWMTaskHandle = NULL;
                }
                
                currentMode = MODE_IDLE;

                Serial.println("[TEST_PWM_CONTROL] Prueba cancelada.");

                break; // Salir del bucle y terminar la tarea
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // Evitar consumo excesivo de CPU
    }
}

/// @brief  Tarea para ejecutar el control de la prueba del PWM en un núcleo específico
/// @param core 
/// @return void
void testPWMControlerTask(uint8_t core) {
   if (testPWMControlerTaskHandle == NULL) {
        xTaskCreatePinnedToCore(
            [](void* pvParameters) {
                testPWMControler();
            },
            "TestPWMControlerTask", // Text name for the task.
            2048,                  // Stack size in words, not bytes.
            NULL,                  // Parameter passed into the task.
            2,                     // Priority at which the task is created.
            &testPWMControlerTaskHandle, 
            core
        );  // Used to pass out the created task's handle.
    }
    
}