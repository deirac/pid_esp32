#include "drivers/pwm.h"
#include "config.h"
#include <driver/mcpwm.h>

static PWMData pwmData;

// =======================================================
// Inicializa el PWM
// =======================================================
void initPWM() {
    // Configuración de pines PWM
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, PWM_H_PIN);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, PWM_L_PIN);

    // Configuración del timer PWM
    mcpwm_config_t pwm_config;
    pwm_config.frequency = PWM_FREQ;
    pwm_config.cmpr_a = 0;  // Inicia en 0%
    pwm_config.cmpr_b = 0;  // Inicia en 0%
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;

    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);

    // DEADTIME
    uint32_t deadtime_ticks = (PWM_DEADTIME_NS * 80) / 1000;

    
    mcpwm_deadtime_enable(
        MCPWM_UNIT_0,
        MCPWM_TIMER_0,
        MCPWM_ACTIVE_RED_FED_FROM_PWMXA,  
        deadtime_ticks,
        deadtime_ticks
    );

    // Establecer ciclo de trabajo inicial en 0%
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 0.0f);
    mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);

    // Inicializar datos del PWM
    pwmData.frequency_hz = PWM_FREQ;
    pwmData.duty_cycle_percent_A = 0;
    pwmData.duty_cycle_percent_B = 0;
    pwmData.sampling_time_ms = PWM_SAMPLING_MS;
    pwmData.dead_time_ns = PWM_DEADTIME_NS;
    pwmData.signal_active = false;
}

// =======================================================
// Obtiene los datos actuales del PWM
// =======================================================
PWMData getPWMData() {
    // Actualizar datos del PWM
    pwmData.duty_cycle_percent_A = mcpwm_get_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A);
    pwmData.duty_cycle_percent_B = mcpwm_get_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B);
    pwmData.signal_active = (pwmData.duty_cycle_percent_A > 0);

    return pwmData;
}

// =======================================================
// Establece el ciclo de trabajo del PWM
// =======================================================
void setPWMDuty(float duty_percent) {
    if (duty_percent < (DUTY_MIN * 100.0f)) duty_percent = DUTY_MIN * 100.0f;
    if (duty_percent > (DUTY_MAX * 100.0f)) duty_percent = DUTY_MAX * 100.0f;

    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, duty_percent);
    mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
}

/// @brief Rampa el ciclo de trabajo del PWM desde un valor inicial hasta un valor final en un tiempo determinado
/// @param start_duty 
/// @param end_duty 
/// @param ramp_time_ms 
void rampPWMDuty(float start_duty, float end_duty, uint32_t ramp_time_ms) {
    if (start_duty < DUTY_MIN) start_duty = DUTY_MIN;
    if (start_duty > DUTY_MAX) start_duty = DUTY_MAX;
    if (end_duty < DUTY_MIN) end_duty = DUTY_MIN;
    if (end_duty > DUTY_MAX) end_duty = DUTY_MAX;

    float step = (end_duty - start_duty) / (ramp_time_ms / 10.0f); // Paso cada 10 ms
    float current_duty = start_duty;

    while ((step > 0 && current_duty < end_duty) || (step < 0 && current_duty > end_duty)) {
        setPWMDuty(current_duty * 100.0f);
        current_duty += step;
        delay(10);
    }
    setPWMDuty(end_duty * 100.0f); // Asegurar que termine en el valor exacto
}
// =======================================================
// Imprime los datos del PWM en el monitor serial
// =======================================================
void printPWMData() {
    Serial.println("[PWM DATA]");
    Serial.print("Frecuencia (Hz): ");
    Serial.println(pwmData.frequency_hz);
    Serial.print("Ciclo de trabajo A (%): ");
    Serial.println(pwmData.duty_cycle_percent_A);
    Serial.print("Ciclo de trabajo B (%): ");
    Serial.println(pwmData.duty_cycle_percent_B);
    Serial.print("Tiempo de muestreo (ms): ");
    Serial.println(pwmData.sampling_time_ms);
    Serial.print("Tiempo muerto (ns): ");
    Serial.println(pwmData.dead_time_ns);
    Serial.print("Señal activa: ");
    Serial.println(pwmData.signal_active ? "Sí" : "No");
}