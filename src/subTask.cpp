#include "subTask.h"
#include "tasks/testEncoder.h"
#include "tasks/testPWM.h"
#include "tasks/testPID.h"
#include "tasks/calibrate.h"
#include "tasks/runMotor.h"

// --------------------------------------------------
// -------- handle sub-tasks implementations --------
// --------------------------------------------------
static TaskHandle_t testPIDTaskHandle = NULL;
static TaskHandle_t testPWMTaskHandle = NULL;
static TaskHandle_t calibrateTaskHandle = NULL;
static TaskHandle_t runMotorTaskHandle = NULL;
// ---------------------------------------------------


// ======== TEST ENCODER ================
void startTestEncoderTask() {
    testEncoderTask(1);
    testEncoderControlTask(0);
}

// ======== TEST PWM ===================
void startTestPWMTask() {
    if(testPWMTaskHandle == NULL) {
        xTaskCreate(
            testPWMTask,             // Function that implements the task.
            "TestPWMTask",           // Text name for the task.
            2048,                    // Stack size in words, not bytes.
            NULL,                    // Parameter passed into the task.
            1,                       // Priority at which the task is created.
            &testPWMTaskHandle);     // Used to pass out the created task's handle.
    }
}
void stopTestPWMTask() {
    if(testPWMTaskHandle != NULL) {
        vTaskDelete(testPWMTaskHandle);
        testPWMTaskHandle = NULL;
    }
}

// ======== TEST PID CONTROLLER =========
void startTestPIDTask() {
    if(testPIDTaskHandle == NULL) {
        xTaskCreate(
            testPIDTask,             // Function that implements the task.
            "TestPIDTask",           // Text name for the task.
            2048,                    // Stack size in words, not bytes.
            NULL,                    // Parameter passed into the task.
            1,                       // Priority at which the task is created.
            &testPIDTaskHandle);     // Used to pass out the created task's handle.
    }
}
void stopTestPIDTask() {
    if(testPIDTaskHandle != NULL) {
        vTaskDelete(testPIDTaskHandle);
        testPIDTaskHandle = NULL;
    }
}

// ======== CALIBRATE ==================
void startCalibrateTask() {
    if(calibrateTaskHandle == NULL) {
        xTaskCreate(
            calibrateTask,           // Function that implements the task.
            "CalibrateTask",         // Text name for the task.
            2048,                    // Stack size in words, not bytes.
            NULL,                    // Parameter passed into the task.
            1,                       // Priority at which the task is created.
            &calibrateTaskHandle);   // Used to pass out the created task's handle.
    }
}
void stopCalibrateTask() {
    if(calibrateTaskHandle != NULL) {
        vTaskDelete(calibrateTaskHandle);
        calibrateTaskHandle = NULL;
    }
}

// ======== RUN MOTOR ==================
void startRunMotorTask() {
    if(runMotorTaskHandle == NULL) {
        xTaskCreate(
            runMotorTask,            // Function that implements the task.
            "RunMotorTask",          // Text name for the task.
            2048,                    // Stack size in words, not bytes.
            NULL,                    // Parameter passed into the task.
            1,                       // Priority at which the task is created.
            &runMotorTaskHandle);    // Used to pass out the created task's handle.
    }
}
void stopRunMotorTask() {
    if(runMotorTaskHandle != NULL) {
        vTaskDelete(runMotorTaskHandle);
        runMotorTaskHandle = NULL;
    }
}
