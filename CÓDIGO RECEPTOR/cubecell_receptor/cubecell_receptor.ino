#include "LoRaWan_APP.h"
#include "Arduino.h"

// ====================== CLAVES OTAA (Receptor) ======================
uint8_t devEui[] = { 0x70, 0xC3, 0xQ5, 0x7E, 0xC0, 0x08, 0xFD, 0x09 };  // DevEUI único para receptor
uint8_t appEui[] = { 0x70, 0xF3, 0xD5, 0x7E, 0xD0, 0x02, 0x7A, 0xC3 };   // Nuevo JoinEUI para receptor
uint8_t appKey[] = { 0x16, 0x09, 0x04, 0x18, 0xFE, 0x32, 0x2A, 0x06, 0x6B, 0xCD, 0x9C, 0x06, 0xD8, 0x07, 0xDF, 0xAD };   // AppKey de ejemplo para receptor

// --------- DEFINICIONES DUMMY (para OTAA, no se usan) ---------
uint32_t devAddr = 0;
uint8_t nwkSKey[16] = { 0 };
uint8_t appSKey[16] = { 0 };

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

// Ciclo de transmisión: el receptor envía un uplink dummy para abrir su ventana de recepción
uint32_t appTxDutyCycle = 30000;

void downLinkDataHandle(McpsIndication_t *mcpsIndication)
{
    Serial.printf("Downlink received: %d bytes on port %d\r\n", mcpsIndication->BufferSize, mcpsIndication->Port);
    for (uint8_t i = 0; i < mcpsIndication->BufferSize; i++) {
        Serial.printf("%02X ", mcpsIndication->Buffer[i]);
    }
    Serial.println();
}

void prepareTxFrame(uint8_t port)
{
    // En este ejemplo, el receptor envía un uplink mínimo para abrir su ventana de recepción
    appDataSize = 1;
    appData[0] = 0x00;
}

void setup()
{
    Serial.begin(115200);
    deviceState = DEVICE_STATE_INIT;
    LoRaWAN.ifskipjoin();
}

void loop()
{
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
