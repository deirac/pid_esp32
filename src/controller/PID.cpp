#include "Controller/PID.h"
#include "config.h"

// ============================================
// Inicialización del PID
// ============================================

void initPID(PIDData* pid){

    pid->Kp = PID_KP;
    pid->Ki = PID_KI;
    pid->Kd = PID_KD;

    pid->setpoint_rpm = PID_SETPOINT_RPM;
    pid->sampling_time_ms = PID_SAMPLING_MS;

    pid->output_min = PID_OUTPUT_MIN;
    pid->output_max = PID_OUTPUT_MAX;

    // Estado interno
    pid->error = 0.0f;
    pid->last_error = 0.0f;
    pid->integral = 0.0f;
    pid->derivative = 0.0f;
    pid->feedback_rpm = 0.0f;

    pid->output_pwm = 0.0f;
}

// ============================================
// Cálculo del PID
// ============================================

float computePID(PIDData* pid, float measurement, float dt){

    // Error
    pid->error = pid->setpoint_rpm - measurement;

    // Proporcional
    float P = pid->Kp * pid->error;

    // Integral
    pid->integral += pid->Ki * pid->error * dt;

    // Anti-windup: limitar la integral **solo si la salida saturó**
    float raw_output = P + pid->integral;

    if (raw_output > pid->output_max) {
        raw_output = pid->output_max;
        // si el error empuja hacia la saturación, no integramos
        if (pid->error > 0) pid->integral -= pid->Ki * pid->error * dt;
    }
    else if (raw_output < pid->output_min) {
        raw_output = pid->output_min;
        if (pid->error < 0) pid->integral -= pid->Ki * pid->error * dt;
    }

    float I = pid->integral;

    // Derivada
    pid->derivative = (pid->error - pid->last_error) / dt;
    float D = pid->Kd * pid->derivative;

    // Salida final
    pid->output_pwm = P + I + D;

    // Saturación final
    if (pid->output_pwm > pid->output_max) pid->output_pwm = pid->output_max;
    if (pid->output_pwm < pid->output_min) pid->output_pwm = pid->output_min;

    pid->last_error = pid->error;

    return pid->output_pwm;
}

// ============================================
// Reset del estado interno
// ============================================

void resetPID(PIDData* pid){

    pid->error = 0.0f;
    pid->last_error = 0.0f;
    pid->integral = 0.0f;
    pid->derivative = 0.0f;
    pid->feedback_rpm = 0.0f;

    pid->output_pwm = 0.0f;
}
