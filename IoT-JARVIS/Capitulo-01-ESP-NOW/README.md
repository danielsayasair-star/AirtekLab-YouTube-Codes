# IoT-JARVIS — Capítulo 1: ESP-NOW

En este capítulo construimos una comunicación directa entre dos ESP32. Primero enviamos números, después transmitimos la temperatura de un DS18B20 y finalmente usamos ese dato para controlar un cooler de 12 V mediante un módulo de relé.

## Orden de las pruebas

1. `01-Obtener-MAC/Obtener_MAC.ino`: cargar en cada placa para consultar su MAC de estación.
2. `02-Prueba-Basica/Transmisor/Transmisor.ino`: cargar en el ESP32 convencional.
3. `02-Prueba-Basica/Receptor/Receptor.ino`: cargar en el ESP32-C3 SuperMini.
4. `03-DS18B20/Transmisor/Transmisor_DS18B20.ino`: transmisor con DS18B20 y LCD I2C.
5. `03-DS18B20/Receptor/Receptor_Temperatura.ino`: receptor que muestra la temperatura.
6. `04-Control-Cooler/Receptor_Control_Cooler.ino`: reemplaza el código del receptor para controlar el cooler.

## Conexiones principales

- DS18B20: GPIO 4 del ESP32 transmisor.
- LCD I2C: dirección `0x27`, 16 × 2.
- Entrada del módulo de relé: GPIO 5 del ESP32-C3.
- Cooler: alimentación externa de 12 V; no alimentarlo desde la placa ESP32.

## Notas

Los ejemplos se probaron durante el desarrollo con Arduino-ESP32 3.3.12 y el receptor incluye compatibilidad con las firmas de callback de las versiones 2.x y 3.x. En el ESP32-C3, si el monitor serie no muestra mensajes, habilita `Herramientas > USB CDC On Boot > Enabled`.

Las MAC del ejemplo corresponden a las placas usadas en el video. Si usas otras, consulta la MAC de cada placa y actualiza `macReceptor` en los transmisores.

El control del cooler enciende a partir de 25 °C y apaga a 24 °C o menos. Si deja de recibir datos durante 5 segundos después de la primera lectura, apaga la salida. Verifica la lógica del relé y las conexiones antes de energizar el circuito. Esta demostración no sustituye las protecciones necesarias para equipos reales.

El enlace al video se agregará cuando esté publicado.