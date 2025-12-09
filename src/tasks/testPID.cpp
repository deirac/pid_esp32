#include "tasks/testPID.h"
#include "controller/PID.h"
#include "config.h"


TaskHandle_t testPIDTaskHandle = NULL;
TaskHandle_t testPIDControlTaskHandle = NULL;

// =========================================
// --------------- Test PID  ---------------
// =========================================

/// @brief Función principal de la prueba del PID
/// @param void
/// @return void
void testPID() {
    Serial.println("[TEST_PID] Tarea iniciada.");
    
    PIDData* pid;
    initPID(pid);
    vTaskDelay(pdTICKS_TO_MS(100)); // Esperar a que el PID se estabilice

}

/// @brief 
/// @param pvParameters 
void testPIDTask(uint8_t core) {
   if(testPIDTaskHandle == NULL){
        xTaskCreatePinnedToCore(
            [](void* pvParameters){
                testPID();
            },
                "TestPID",
                2048,
                NULL,
                2,
                &testPIDTaskHandle,
                core
        );
   }
}


// ==============================================
// ---------------- test control PID-------------
// ==============================================

/*
* @brief Función que controla la prueba del PID
* @param void
* @return void
*/
void testPIDControl(){
    Serial.println("[TEST_PID] Tarea iniciada.");
    Serial.println("Presione 'c' para cancelar la prueba del PID.");

    while(true){
        if(Serial.available()>0){
            char input =Serial.read();

            if(input == 'c'){
                Serial.println("[TEST_PID] Candelando prueba del PID");

                if(testPIDTaskHandle != NULL){
                    vTaskDelete(testPIDTaskHandle);
                    testPIDTaskHandle = NULL;
                }

                currentMode = MODE_IDLE;

                Serial.println("[TEST_PID] Prueba cancelada.");

                break; // Salir del bucle y terminar la tarea
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // Evitar consumo excesivo de CPU
    }
}

/// @brief  Tarea para ejecutar el control de la prueba del PID en un núcleo específico
/// @param core 
/// @return void
void testPIDControlerTask(uint8_t core){
    if(testPIDControlTaskHandle == NULL){
        xTaskCreatePinnedToCore(
            [](void* pvParameters) {
                testPIDControl();
            },
            "TestPIDControlerTask",
            2048,
            NULL,
            2,
            &testPIDControlTaskHandle,
            core
        );
    }
}