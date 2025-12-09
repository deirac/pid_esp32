#include "drivers/encoder.h"
#include "config.h"

static volatile uint32_t lastPulseTimeUs = 0;
static volatile uint32_t pulseIntervalUs = 0;
static volatile bool newPulse = false;

static EncoderData enc;   

// =======================================================
// ISR — Se ejecuta en cada flanco del canal B
// =======================================================
static void IRAM_ATTR encoderISR() {// Leer tiempo actual y calcular intervalo
    uint32_t now = micros();
    uint32_t dt = now - lastPulseTimeUs;

    pulseIntervalUs = dt;
    lastPulseTimeUs = now;
    newPulse = true;

    // Guardar datos en la estructura
    enc.last_pulse_us = now;
    enc.pulse_interval_us = dt;
    enc.last_valid_time = now;
}

// =======================================================
// Inicialización del encoder
// =======================================================
void initEncoder() {// Configurar pin y variables
    pinMode(ENCODER_PIN_B, INPUT_PULLUP);

    enc.ppr = ENCODER_PPR;
    enc.min_interval_us = ENCODER_MIN_DT_US;
    enc.max_interval_us = ENCODER_MAX_DT_US;
    enc.timeout_ms = ENCODER_TIMEOUT_MS;
    enc.sampling_time_ms = ENCODER_SAMPLING_MS;

    enc.signal_valid = false;
    enc.timeout_active = true;
    enc.rpm_instant = 0;
    enc.rpm_filtered = 0;
    enc.frequency_hz = 0;
    enc.last_pulse_us = micros();
    enc.last_valid_time = micros();

    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), encoderISR, RISING);
}


// =======================================================
// Obtener datos actuales del encoder
// =======================================================
EncoderData getEncoderData() { // Lectura segura de variables compartidas

    noInterrupts();
    bool pulseReceived = newPulse;
    uint32_t interval = pulseIntervalUs;
    newPulse = false;
    interrupts();

    uint32_t now = micros();

    // -------- Timeout del encoder --------
    if ((now - enc.last_valid_time) > enc.timeout_ms * 1000UL) {
        enc.timeout_active = true;
        enc.frequency_hz = 0;
        enc.rpm_instant = 0;
        enc.signal_valid = false;
        return enc;
    }

    enc.timeout_active = false;

    // -------- Validación del intervalo --------
    if (pulseReceived &&
        interval >= enc.min_interval_us &&
        interval <= enc.max_interval_us) {

        enc.signal_valid = true;

        enc.frequency_hz = 1e6f / (float)interval;
        enc.rpm_instant = (enc.frequency_hz * 60.0f) / enc.ppr;

        // Pequeño filtro exponencial
        // y[k] = α * x[k] + (1 - α) * y[k-1]
        enc.rpm_filtered = 0.85f * enc.rpm_filtered + 0.15f * enc.rpm_instant;

    } else {
        enc.signal_valid = false;
        // NO seteamos RPM en cero mientras no haya timeout
    }

    return enc;
}


// =======================================================
// Imprimir datos del encoder
// =======================================================
void printEncoderData(const EncoderData& d) {// Mostrar en Serial los datos del encoder
    Serial.println("=== Encoder Data ===");
    
    Serial.print("Señal válida: ");
    Serial.println(d.signal_valid ? "Sí" : "No");

    Serial.print("Timeout: ");
    Serial.println(d.timeout_active ? "Sí" : "No");

    Serial.print("Frecuencia (Hz): ");
    Serial.println(d.frequency_hz);

    Serial.print("RPM instantánea: ");
    Serial.println(d.rpm_instant);

    Serial.print("RPM filtrada: ");
    Serial.println(d.rpm_filtered);

    Serial.print("Intervalo entre pulsos (us): ");
    Serial.println(d.pulse_interval_us);

    Serial.print("Último pulso (us): ");
    Serial.println(d.last_pulse_us);

    Serial.print("Último tiempo válido (us): ");
    Serial.println(d.last_valid_time);

    Serial.print("Tiempo de muestreo (ms): ");
    Serial.println(d.sampling_time_ms);
}
