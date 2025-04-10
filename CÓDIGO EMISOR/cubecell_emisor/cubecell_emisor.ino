#include "LoRaWan_APP.h"
#include "Arduino.h"

// ====================== CLAVES OTAA (Emisor) ======================
uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x06, 0xFC, 0xFF };
uint8_t appEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x03, 0x4B, 0xDA };
uint8_t appKey[] = { 0xAC, 0x5F, 0x87, 0x36, 0xD6, 0x97, 0x9E, 0x5B, 0x2A, 0xF9, 0x23, 0xFB, 0xCB, 0xE9, 0x20, 0xFD };

// --------- DEFINICIONES DUMMY (para OTAA, no se usan) ---------
uint32_t devAddr = 0;           // Dummy: no se usa en OTAA
uint8_t nwkSKey[16] = { 0 };      // Dummy: no se usa en OTAA
uint8_t appSKey[16] = { 0 };      // Dummy: no se usa en OTAA

// ====================== CONFIGURACIÓN ======================
uint16_t userChannelsMask[6] = { 0x00FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };
LoRaMacRegion_t loraWanRegion = LORAMAC_REGION_EU868;
DeviceClass_t loraWanClass = CLASS_A;

// Configuración de red
bool overTheAirActivation = true;
bool loraWanAdr = true;
bool keepNet = false;
bool isTxConfirmed = false;
uint8_t appPort = 2;
uint8_t confirmedNbTrials = 4;

// Ciclo de transmisión (en milisegundos)
uint32_t appTxDutyCycle = 15000;

// ====================== FUNCIONES ======================
void prepareTxFrame(uint8_t port)
{
    // En este ejemplo, enviamos "Hello" (código ASCII)
    appDataSize = 5;
    appData[0] = 0x48; // 'H'
    appData[1] = 0x65; // 'e'
    appData[2] = 0x6C; // 'l'
    appData[3] = 0x6C; // 'l'
    appData[4] = 0x6F; // 'o'
}

void setup() {
    Serial.begin(115200);
    deviceState = DEVICE_STATE_INIT;
    LoRaWAN.ifskipjoin();
}

void loop() {
    switch (deviceState)
    {
        case DEVICE_STATE_INIT:
#if (LORAWAN_DEVEUI_AUTO)
            LoRaWAN.generateDeveuiByChipID();
#endif
            printDevParam();
            LoRaWAN.init(loraWanClass, loraWanRegion);
            deviceState = DEVICE_STATE_JOIN;
            break;
            
        case DEVICE_STATE_JOIN:
            LoRaWAN.join();
            break;
            
        case DEVICE_STATE_SEND:
            prepareTxFrame(appPort);
            LoRaWAN.send();
            deviceState = DEVICE_STATE_CYCLE;
            break;
            
        case DEVICE_STATE_CYCLE:
            // Usamos directamente la expresión, evitando definir txDutyCycleTime extra.
            LoRaWAN.cycle(appTxDutyCycle + randr(0, APP_TX_DUTYCYCLE_RND));
            deviceState = DEVICE_STATE_SLEEP;
            break;
            
        case DEVICE_STATE_SLEEP:
            LoRaWAN.sleep();
            break;
            
        default:
            deviceState = DEVICE_STATE_INIT;
            break;
    }
}
