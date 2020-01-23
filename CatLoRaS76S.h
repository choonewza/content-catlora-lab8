#ifndef CatLoRaS76S_h
#define CatLoRaS76S_h
#include "Arduino.h"
#include <Wire.h>
#include <HTS221.h>
#include "wiring_private.h" // pinPeripheral() function
class CatLoRaS76S
{
private:
    void SERCOM1_Handler(void);
    void macCommand(String comm);
    void freqLoraConfig(void);

public:
    ~CatLoRaS76S();
    CatLoRaS76S();
    void begin(uint32_t bundRate);
    void joinABP(String loraClass, String devEui, String devAddr, String nwksKey, String appsKey);
    void joinOTAA(String loraClass, String devEui, String appEui, String appKey);
    void transmit(uint8_t port, String payLoad);
    String receive(void);
    unsigned int getPortReceive(String dataFromReceive);
    String getPayloadReceive(String dataFromReceive);
    double getTemp();
    double getHumi();
    String getTempHumiCayenneLPPformat();
};
#endif