#include "menuTask.h"
#include "menuManager.h"

TaskHandle_t menuTaskHandle = NULL;
volatile SystemMode currentMode = MODE_IDLE;

/// @brief Tarea del menú principal
/// @param parameter
/// @return void  
void menuTask(void* parameter) {
    vTaskDelay(pdMS_TO_TICKS(500));
    printMenu();

    while (true) {
        SystemMode selectedMode = handleUserInput();

        if (selectedMode != MODE_IDLE) {
            currentMode = selectedMode;

            Serial.print(F("\n Modo seleccionado: "));
            Serial.println(getModeName(currentMode));
            vTaskDelay(pdMS_TO_TICKS(500));
            Serial.println(F("Cerrando menú...\n"));
            vTaskDelay(pdMS_TO_TICKS(500));

            deleteMenuTask();
        }
        else {
            vTaskDelay(pdMS_TO_TICKS(500));
            Serial.println(F("Por favor, seleccione una opción válida."));
            Serial.print(F("seleccione una opción >>> "));
        }

        vTaskDelay(pdMS_TO_TICKS(50));  // Evita consumo excesivo de CPU
    }
}

/// @brief Inicia la tarea del menú
/// @param void
/// @return void
void startMenuTask() {
    xTaskCreatePinnedToCore(
        menuTask,
        "Menu Task",
        4096,
        NULL,
        1,
        &menuTaskHandle,
        0   
    );
}

void deleteMenuTask() {
    if (menuTaskHandle != NULL) {
        vTaskDelete(menuTaskHandle);
        menuTaskHandle = NULL;
    }
}
