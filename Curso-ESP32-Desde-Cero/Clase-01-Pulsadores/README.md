# Clase 01 — Programación de pulsadores con ESP32

Material complementario del video de AirtekLab sobre programación de pulsadores.

## Hardware
- Placa ESP32 Dev Module.
- Pulsador entre GPIO 18 y GND, usando `INPUT_PULLUP`.
- En los ejercicios con LED: GPIO 25 conectado a una resistencia limitadora y al ánodo del LED; cátodo a GND.
- Monitor Serie a 115200 baudios.

Importante: no conectes un LED a un GPIO sin resistencia limitadora. El ejercicio 03 usa dos pulsadores: GPIO 18 para encender y GPIO 19 para apagar el LED.

## Ejercicios
1. Pulsador momentáneo: el LED permanece encendido mientras se mantiene presionado.
2. Alternar el estado del LED con cada pulsación y antirrebote.
3. Dos pulsadores: uno enciende y otro apaga el LED.
4. Contador de pulsaciones en el Monitor Serie.
5. Modificar una consigna; mantener pulsado repite el incremento.
6. Distinguir pulsaciones cortas y largas.

## Archivos
- [01 — Pulsador momentáneo](./01_Pulsador_Momentaneo.ino)
- [02 — Alternar estado](./02_Alternar_Estado.ino)
- [03 — Dos pulsadores y LED](./03_Dos_Pulsadores_LED.ino)
- [04 — Contador de pulsaciones](./04_Contador_Pulsaciones.ino)
- [05 — Consigna con Monitor Serie](./05_Consigna_Monitor_Serie.ino)
- [06 — Pulsación corta y larga](./06_Pulsacion_Corta_Larga.ino)

Abrí el archivo `.ino` del ejercicio en Arduino IDE, seleccioná ESP32 Dev Module y, cuando corresponda, abrí el Monitor Serie a 115200 baudios.
