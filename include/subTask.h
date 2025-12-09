#pragma once
#include <Arduino.h>

// ======== TEST ENCODER ================
void startTestEncoderTask();
// ======== TEST PWM ===================
void startTestPWMTask();
void stopTestPWMTask();

// ======== TEST PID CONTROLLER =========
void startTestPIDTask();
void stopTestPIDTask();

// ======== CALIBRATE ==================
void startCalibrateTask();
void stopCalibrateTask();

// ======== RUN MOTOR ==================
void startRunMotorTask();
void stopRunMotorTask();