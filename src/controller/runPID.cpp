#include "controller/runPID.h"
#include "controller/PID.h"
#include "drivers/encoder.h"
#include "drivers/pwm.h"
#include "config.h"


TaskHandle_t runPIDTaskHandle = NULL;

void PIDControl(){

    PIDData pid;
    PWMData pwm;

    initPID(&pid);
    initPWM(&pwm);
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
            

            float pwmOutput = computePID(&pid, encoder.rpm_filtered, dt);
            setPWMDuty(pwmOutput); 
        }
    }
}


void startRunPIDTask(uint8_t core){
    if(runPIDTaskHandle == NULL){
        xTaskCreatePinnedToCore(
            [](void* pvParameters){
                PIDControl();
            },
            "PIDControl",
            4096,
            NULL,
            3, 
            &runPIDTaskHandle,
            core
        );
    }
}
