#include "tasks/testPID.h"
#include "controller/PID.h"
#include "drivers/pwm.h"

#include "config.h"
#include <stdlib.h>

TaskHandle_t testPIDTaskHandle = NULL;
TaskHandle_t testPIDControlTaskHandle = NULL;

// Variables globales para la simulación
static float simulatedRPM = 0.0;
static bool testRunning = true;

// =========================================
// --------------- Test PID  ---------------
// =========================================

float injectionErr(float setpoint) {
    float randomFactor = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
    float error = setpoint * 0.20f * randomFactor;   // 20% de error
    return setpoint + error;
}

/// @brief Modelo simple de planta con inercia
/// @param pwmInput Entrada PWM (0-255)
/// @param dt Tiempo delta en segundos
/// @return Nueva velocidad RPM
float simulatePlant(float pwmInput, float dt) {
    static float currentRPM = 0.0;
    const float plantGain = 0.8f;      // Ganancia del motor
    const float timeConstant = 0.5f;   // Constante de tiempo
    
    // Modelo de primer orden: RPM se aproxima a targetRPM
    float targetRPM = pwmInput * plantGain;
    currentRPM += (targetRPM - currentRPM) * (dt / timeConstant);
    
    return currentRPM;
}

/// @brief Función principal de la prueba del PID
void testPID() {
    Serial.println("[TEST_PID] Tarea iniciada.");
    
    // DECLARAR LA INSTANCIA DEL PID (no puntero doble)
    PIDData pid;  // Instancia directa
    initPID(&pid);  // Pasar dirección de la instancia
    
    PWMData pwm;
    initPWM(&pwm);

    // Variables para control de tiempo
    uint32_t lastTime = millis();
    uint32_t currentTime;
    float dt;
    
    // Inicializar generador aleatorio
    srand(millis());
    
    Serial.println("Tiempo(ms), Setpoint, RPM, PWM, Error");
    
    testRunning = true;
    uint32_t iteration = 0;
    
    while(testRunning) {
        currentTime = millis();
        dt = (currentTime - lastTime) / 1000.0f;  // Convertir a segundos
        
        if(dt > 0.001f) {  // Solo procesar si ha pasado tiempo significativo
            // 1. Generar perturbación en la medición
            float measuredRPM = injectionErr(simulatedRPM);
            
            // 2. Calcular salida del PID
            float pidOutput = computePID(&pid, measuredRPM, dt);
            
            // 3. Simular planta (motor) con la salida del PID
            simulatedRPM = simulatePlant(pidOutput, dt);
            
            // 4. Calcular error real para logging
            float actualError = pid.setpoint_rpm - simulatedRPM;
            
            // 5. Logging para análisis
            Serial.print(currentTime);
            Serial.print(",  ");
            Serial.print(pid.setpoint_rpm);
            Serial.print(",  ");
            Serial.print(simulatedRPM, 2);
            Serial.print(",  ");
            Serial.print(pidOutput, 1);
            Serial.print(", ");
            Serial.println(actualError, 2);
            
            // 6. Actualizar tiempo
            lastTime = currentTime;
            iteration++;
            
            // Cambiar setpoint periódicamente para pruebas
            if(iteration == 100) {
                pid.setpoint_rpm = 750.0f;
                Serial.println("[TEST_PID] Setpoint cambiado a 750 RPM");
            } else if(iteration == 200) {
                pid.setpoint_rpm = 1100.0f;
                Serial.println("[TEST_PID] Setpoint cambiado a 1100 RPM");
            } else if(iteration == 300) {
                pid.setpoint_rpm = 400.0f;
                Serial.println("[TEST_PID] Setpoint cambiado a 400 RPM");
            }
            
            // Reiniciar contador
            if(iteration > 400) iteration = 0;
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        
        // Pausa mínima para no saturar
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    
    Serial.println("[TEST_PID] Tarea finalizada.");
    vTaskDelete(NULL);
}

/// @brief Iniciar tarea de prueba PID
void testPIDTask(uint8_t core) {
    if(testPIDTaskHandle == NULL) {
        xTaskCreatePinnedToCore(
            [](void* pvParameters) {
                testPID();
            },
            "TestPID",
            4096,
            NULL,
            2,
            &testPIDTaskHandle,
            core
        );
        Serial.printf("[TEST_PID] Tarea creada en core %d\n", core);
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
void testPIDControl() {
    Serial.println("[TEST_PID_CONTROL] Tarea iniciada.");
    Serial.println("Comandos: 'c'=cancelar, 's'=estado, 'p'=cambiar setpoint");
    
    String inputBuffer = "";
    
    while(true) {
        if(Serial.available() > 0) {
            char input = Serial.read();
            
            if(input == '\n' || input == '\r') {
                if(inputBuffer.length() > 0) {
                    // Procesar comando
                    if(inputBuffer.charAt(0) == 'c') {
                        Serial.println("[TEST_PID] Cancelando prueba...");
                        testRunning = false;
                        
                        vTaskDelay(pdMS_TO_TICKS(100));
                        
                        if(testPIDTaskHandle != NULL) {
                            vTaskDelete(testPIDTaskHandle);
                            testPIDTaskHandle = NULL;
                        }
                        
                        currentMode = MODE_IDLE;
                        Serial.println("[TEST_PID] Prueba cancelada.");
                        
                        break;
                    } 
                    else if(inputBuffer.charAt(0) == 's') {
                        Serial.println("[TEST_PID] Estado:");
                        Serial.printf("  Test activo: %s\n", testRunning ? "SI" : "NO");
                        Serial.printf("  RPM simulado: %.1f\n", simulatedRPM);
                    }
                    else if(inputBuffer.charAt(0) == 'p') {
                        // Cambiar setpoint: p120
                        float newSetpoint = inputBuffer.substring(1).toFloat();
                        if(newSetpoint > 0) {
                            Serial.printf("[TEST_PID] Setpoint cambiado a: %.1f RPM\n", newSetpoint);
                            // Aquí necesitarías acceso al PID para cambiar setpoint
                        }
                    }
                    
                    inputBuffer = "";
                }
            } else {
                inputBuffer += input;
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(50));
    }
    
    vTaskDelete(NULL);
}

/// @brief Tarea para ejecutar el control de la prueba del PID
void testPIDControlerTask(uint8_t core) {
    if(testPIDControlTaskHandle == NULL) {
        xTaskCreatePinnedToCore(
            [](void* pvParameters) {
                testPIDControl();
            },
            "TestPIDControlerTask",
            2048,
            NULL,
            1,
            &testPIDControlTaskHandle,
            core
        );
        Serial.printf("[TEST_PID_CONTROL] Tarea creada en core %d\n", core);
    }
}