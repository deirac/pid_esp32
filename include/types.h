#pragma once
#include <stdint.h>

// ---------------------------------------------------------
// Modos de operación del sistema
// ---------------------------------------------------------
typedef enum {
    MODE_IDLE = 0,
    MODE_TEST_ENCODER,      // Modo de prueba del codificador
    MODE_TEST_PWM,          // Modo de prueba del PWM
    MODE_TEST_PID,          // Modo de prueba del controlador PI
    MODE_CALIBRATION,       // Modo de calibración
    MODE_FORWARD            // Modo de operación normal hacia adelante
} SystemMode;


// ---------------------------------------------------------
// Estado del Encoder
// ---------------------------------------------------------
typedef struct {
    // configuración
    uint32_t ppr;               // Pulsos por revolución
    uint32_t min_interval_us;   // Intervalo mínimo entre pulsos (μs)
    uint32_t max_interval_us;   // Intervalo máximo entre pulsos (μs)
    uint32_t timeout_ms;        // Tiempo de espera para considerar el motor detenido (ms)

    // estado del sistema 
    bool signal_valid;          // Señal válida
    bool timeout_active;        // Timeout activo

    // datos de velocidad
    float rpm_instant;          // RPM instantánea
    float rpm_filtered;         // RPM filtrada (0, 1750)
    float frequency_hz;         // Frecuencia actual (Hz)

    // datos temporales
    uint32_t last_pulse_us;     // Tiempo del último pulso (μs)
    uint32_t pulse_interval_us; // Intervalo entre pulsos (μs)
    uint32_t last_valid_time;   // Último tiempo válido (μs)
    uint32_t sampling_time_ms;   // Tiempo de muestreo (μs)
    
} EncoderData;

// ---------------------------------------------------------
// Estado del PWM
// ---------------------------------------------------------
typedef struct {
    // configuración
    uint32_t frequency_hz;          // Frecuencia del PWM (Hz)
    uint8_t duty_cycle_percent_A;   // Ciclo de trabajo (%)
    uint8_t duty_cycle_percent_B;   // Ciclo de trabajo (%)
    uint32_t sampling_time_ms;      // Tiempo de muestreo (ms)
    uint32_t dead_time_ns;          // Tiempo muerto (μs)

    // estado del sistema
    bool signal_active;             // Señal activa

} PWMData;

// ---------------------------------------------------------
// Estado del PID   
// ---------------------------------------------------------
typedef struct {
    float Kp;                   // Ganancia proporcional
    float Ki;                   // Ganancia integral
    float Kd;                   // Ganancia derivativa

    float error;                // Error actual
    float last_error;           // Último error
    float integral;             // Término integral acumulado
    float derivative;           // Término derivativo   

    float feedback_rpm;         // RPM medidas
    float setpoint_rpm;         // Referencia de RPM

    float output_pwm;           // Salida del controlador (valor PWM)
    float output_min;           // Límite mínimo de la salida
    float output_max;           // Límite máximo de la salida

    uint32_t sampling_time_ms;  // Tiempo de muestreo (ms)
} PIDData;
