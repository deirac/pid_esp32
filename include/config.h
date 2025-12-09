#pragma once

// ============================================
// MOTOR BALDOR RELIANCE CD6215
// ============================================
#define MOTOR_MAX_RPM          1750     // RPM máximo del motor


// ============================================
// ENCODER E6B2-CWZ6C CONFIGURATION
// ============================================
#define ENCODER_PIN_B          34                                           // Pin GPIO para canal B
#define ENCODER_PPR            2e3                                          // 2000 pulsos por revolución
#define ENCODER_MIN_DT_US      (60e6/ ((MOTOR_MAX_RPM * ENCODER_PPR) ))     // ~17μs
#define ENCODER_MAX_DT_US      1e6                                          // 1 s = 1,000,000 μs
#define ENCODER_TIMEOUT_MS     100                                          // Timeout para velocidad cero
#define ENCODER_SAMPLING_MS    1e3                                          // Tiempo de muestreo para reporte

// ============================================
// PWM CONFIGURATION
// ============================================
#define PWM_H_PIN               25
#define PWM_L_PIN               26
#define PWM_FREQ                20e3        // 20 kHz
#define PWM_DEADTIME_NS         100         // 100 ns
#define DUTY_MIN                0.05f       // 5%
#define DUTY_MAX                0.95f       // 95%
#define PWM_SAMPLING_MS         1000         // Tiempo de muestreo para reporte

// ============================================
// PI CONTROLLER
// ============================================
#define PID_KP                   (1.0f/10.0f)        // Kp = 0.1
#define PID_KI                   (2.0f/100.0f)       // Ki = 0.02
#define PID_KD                   (0.0f/10.f)         // Kd = 0.0
#define PID_OUTPUT_MIN           DUTY_MIN
#define PID_OUTPUT_MAX           DUTY_MAX
#define PID_SAMPLING_MS          10                  // Tiempo de muestreo del controlador PID
#define PID_SETPOINT_RPM         900                 // Setpoint de velocidad en RPM para la prueba del PID
#define PID_TIME_US              100                 // Realiza el PID cada 100 us.