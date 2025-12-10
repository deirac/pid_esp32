#include "controller/runPID.h"
#include "controller/PID.h"
#include "drivers/encoder.h"
#include "drivers/pwm.h"
#include "config.h"

TaskHandle_t runPWMTaskHandle = NULL;
TaskHandle_t runPIDTaskHandle = NULL;


volatile float pwmOutput;


// ==========================================
// --------------- PWM Control --------------
// ==========================================
/**
 * @brief Control PWM
 * @param void
 * @return void
 */
void PWMControl(){
    
    initPWM();
    while(true){
        pwmOutput*=100.0f;
        setPWMDuty(pwmOutput);
        Serial.print("salida: ");
        Serial.println(pwmOutput);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    
}

/**
 * @brief Tarea para control PWM
 * @param core 
 * @return void
 */
void startRunPWMTask(uint8_t core){
    if(runPWMTaskHandle == NULL){
        xTaskCreatePinnedToCore(
            [](void* pvParameters){
                PWMControl();
            },
            "PWMControl",
            4096,
            NULL,
            3, 
            &runPWMTaskHandle,
            core
        );
    }
}

// =======================================
// ------------ PID Control --------------
// =======================================
/**
 * @brief Control PID y lectura del encoder 
 * @param void
 * @return void
 */
void PIDControl(){

    PIDData pid;
    initPID(&pid);
    initEncoder();

    //variables de control
    uint32_t startTime = millis();
    int32_t lastControlTime = startTime;
    uint32_t lastPrintTime = startTime;
    bool running = true;


     // ==========  BUCLE PRINCIPAL =========
    while(running) {
        uint32_t currentTime = millis();
        float dt = (currentTime - lastControlTime) / 1e3f; // segundos

        if(dt >= PID_TIME_US/1e3f){// tiempo en ms
            EncoderData encoder = getEncoderData();
            pwmOutput = computePID(&pid, encoder.rpm_filtered, dt);
            Serial.print("rpm filtered: ");
            Serial.println(encoder.rpm_filtered);
        }
        
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

/**
 * @brief Tarea para control PID
 * @param core 
 * @return void
 */
void startRunPIDTask(uint8_t core){
    if(runPIDTaskHandle == NULL){
        xTaskCreatePinnedToCore(
            [](void* pvParameters){
                PIDControl();
            },
            "PIDControl",
            4096,
            NULL,
            2, 
            &runPIDTaskHandle,
            core
        );
    }
}
