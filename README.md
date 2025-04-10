# CubeCell LoRaWAN – Comunicación con TTN y Gateway Dragino LPS8v2

Este proyecto demuestra cómo conectar dos dispositivos Heltec CubeCell HTCC-AB02 a través de LoRaWAN, utilizando un gateway Dragino LPS8v2 y la red de The Things Network (TTN). Se implementa la activación OTAA para registrar los dispositivos y enviar datos periódicos.

---

## Requisitos

- 2 placas Heltec CubeCell HTCC-AB02  
- 2 antenas LoRa (una por placa)  
- 1 Gateway Dragino LPS8v2  
- Cuenta gratuita en The Things Network  
- Arduino IDE con soporte para CubeCell  

---

## Contenido del repositorio

- **CÓDIGO EMISOR/** – Código Arduino del CubeCell que actúa como emisor LoRaWAN.  
- **CÓDIGO RECEPTOR/** – Código Arduino del CubeCell que actúa como receptor de los mensajes enviados por el emisor, usando downlink.  
- **SCRIPT MQTT/** – Script en Python que escucha los mensajes uplink del emisor en TTN y los reenvía como downlink al receptor.  
- **DOCUMENTACIÓN_CUBECELL_LORAWAN_TTN.pdf** – Guía paso a paso que explica cómo configurar el gateway Dragino, registrar los dispositivos CubeCell en TTN, habilitar MQTT, y realizar pruebas de comunicación.  
- **README.md** – Este archivo.

---

## Funcionalidad

### Emisor (CubeCell)

- Se configura con las claves OTAA (DevEUI, JoinEUI, AppKey) obtenidas de TTN.  
- Se conecta automáticamente a la red TTN usando el gateway Dragino.  
- Envía datos cada 15 segundos a través de LoRaWAN.

### Receptor (CubeCell)

- El gateway Dragino reenvía los paquetes recibidos al servidor de TTN.  
- El script MQTT toma esos mensajes uplink y los reenvía como downlink al receptor.  
- El receptor muestra los mensajes recibidos por pantalla (y por monitor serie si se desea).

---

## Para más detalles

Consulta la documentación paso a paso en **DOCUMENTACIÓN_CUBECELL_LORAWAN_TTN.pdf**, que incluye:

- Cómo registrar el gateway Dragino LPS8v2 en TTN.  
- Cómo crear una aplicación y añadir end devices (CubeCell) con OTAA.  
- Cómo configurar las claves en el archivo `.ino`.  
- Cómo compilar y cargar el sketch desde Arduino IDE.  
- Cómo instalar y ejecutar el script Python (MQTT).  
- Cómo usar el "Payload Formatter" para visualizar correctamente los datos.  
- Verificación de datos en el panel TTN.

---

## Estado actual

- Gateway conectado a TTN  
- CubeCell-1 y CubeCell-2 registrados y funcionando con OTAA  
- Comunicación bidireccional verificada en TTN (uplink y downlink vía MQTT)  
- Visualización del mensaje decodificado en la consola TTN

---

## Licencia

Este proyecto está bajo la licencia MIT. Puedes modificarlo y compartirlo libremente.
