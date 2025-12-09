
# PID para Motor DC con driver IR2110, encoder y ESP32

Proyecto: **pid_esp32**  
Descripción: Control PID para ESP32 orientado a controlar un motor DC con encoder. Este repositorio usa PlatformIO y está escrito en C/C++ para el framework de Arduino sobre ESP32.

---

## Contenido del repositorio
- `.vscode/` — configuración del workspace.
- `include/` — archivos de cabecera (`*.h`).
- `lib/` — librerías adicionales.
- `src/` — código fuente principal.
- `test/` — pruebas (si existen).
- `platformio.ini` — configuración de PlatformIO.

> Nota: la lista de archivos y carpetas proviene del repositorio original: `https://github.com/deirac/pid_esp32`.

---

## Requisitos
- ESP32 (cualquier placa compatible con el SDK de Espressif).
- PlatformIO (recomendado) o el entorno de su preferencia que soporte ESP32.
- Fuente de alimentación adecuada para el motor y el ESP32.
- Controlador de motor (driver PWM/MOSFET/H-bridge) compatible con la corriente del motor.
- Encoder (incremental) con lectura por interrupciones o timer.
- Cableado y componentes básicos (resistencias, condensadores, diodos de protección).

---

## Instalación (PlatformIO)
1. Clona el repositorio:
```bash
git clone https://github.com/deirac/pid_esp32.git
cd pid_esp32
```

2. Abre el proyecto en Visual Studio Code con la extensión PlatformIO o usa PlatformIO CLI.

3. Compilar:
```bash
pio run
```

4. Subir al ESP32 (ajusta el puerto serial si es necesario):
```bash
pio run --target upload
```

5. Monitor serial:
```bash
pio device monitor -b 115200
```

---

## Estructura sugerida del código
- `src/main.cpp`: inicialización, bucle principal y llamadas al controlador PID.
- `src/pid.cpp/.h`: implementación del algoritmo PID.
- `src/encoder.cpp/.h`: lectura y filtrado del encoder (RPM).
- `src/pwm_driver.cpp/.h`: configuración y control de PWM para el motor.
- `include/`: cabeceras públicas compartidas.

*(Si los nombres de archivos difieren en el repo, ajuste según corresponda.)*

---

## Conexiones HW (orientativa)
- ESP32 3.3V → alimentación lógica (no alimentar el motor desde la placa).
- GND (ESP32) ↔ GND (Driver) ↔ GND (Fuente de motor).
- PWM pin (ESP32, p. ej. GPIO 18) → entrada PWM del driver.
- Encoder A / B → pines GPIO con interrupciones (p. ej. 34, 35 — entradas ADC-capable no soportan interrupciones en algunos módulos; prefiera GPIOs digitales como 16, 17).
- Vmotor → fuente separada (según especificación del motor).
- Añadir diodo flyback y protección según recomiende el driver.

> **Importante:** Verifique la hoja de datos de su placa ESP32 y del controlador del motor para elegir pines adecuados y niveles lógicos.

---

## Configuración del PID
El proyecto contiene funciones para:
- Leer RPM desde el encoder.
- Filtrar lecturas (promedio o filtro pasabajo).
- Calcular la señal de control PID.
- Aplicar la señal mediante PWM (modulación de ancho de pulso).

### Valores iniciales recomendados
- `Kp`: 0.5 — 2.0
- `Ki`: 0.01 — 0.5
- `Kd`: 0.0 — 0.5

Estos valores dependen mucho de la inercia del motor y la carga. Recomendación de procedimiento de ajuste:
1. Desactivar `Ki` y `Kd`. Incrementar `Kp` hasta que el sistema oscile de forma sostenida (o presente respuesta rápida).
2. Ajustar `Ki` para eliminar error en estado estacionario.
3. Ajustar `Kd` para reducir sobreimpulso y mejorar amortiguamiento.

Puede usar métodos como Ziegler–Nichols o tuning manual.

---

## Telemetría y depuración
- Utilice `Serial` para imprimir RPM, consigna (setpoint), señal PID y PWM.
- Si el proyecto incluye Wi‑Fi o webserver, puede exponer una página simple con valores en tiempo real (opcional).

---

## Pruebas
- Realizar pruebas sin carga (motor sin acoplamiento) para observar respuesta básica.
- Medir RPM con un tacómetro externo para validar lecturas del encoder.
- Testear la inyección de errores (por ejemplo, añadir una perturbación de carga del 20%) para comprobar robustez del control.

---

## Contribuir
1. Haga fork del repo.
2. Cree una rama con su mejora: `git checkout -b feat/nueva-funcion`.
3. Haga commit y push: `git commit -am "Descripción"` / `git push origin feat/nueva-funcion`.
4. Abra un Pull Request describiendo los cambios.

---

## Licencia
Si el repositorio original no especifica licencia, añada una según su preferencia (MIT, Apache-2.0, GPLv3, etc.). Si desea, puedo proponer un texto de licencia.

---

## Contacto
Para dudas o mejoras del README, indique las partes del repositorio que desea que detalle más (por ejemplo: diagrama de pines exacto, ejemplos de código, instrucciones de tuning automáticas).

