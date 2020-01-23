#include "CatLoRaS76S.h"
#define _DEBUG 1
#define _PIN_RX 11
#define _PIN_TX 10
#define _LED_BUIDIN 13
#define _MAC_SIP_RESET "sip reset"
#define _MAC_SET_CH_FREQ "mac set_ch_freq"
#define _MAC_SET_RX2 "mac set_rx2"
#define _MAC_SAVE "mac save"
#define _MAC_SET_CLASS "mac set_class"
#define _MAC_SET_DEVEUI "mac set_deveui"
#define _MAC_SET_DEVADDR "mac set_devaddr"
#define _MAC_SET_NWKSKEY "mac set_nwkskey"
#define _MAC_SET_APPSKEY "mac set_appskey"
#define _MAC_JOIN_ABP "mac join abp"
#define _MAC_TX_UCNF "mac tx ucnf"
#define _MAC_SET_APPEUI "mac set_appeui"
#define _MAC_SET_APPKEY "mac set_appkey"
#define _MAC_JOIN_OTAA "mac join otaa"
Uart Serial2(&sercom1, _PIN_RX, _PIN_TX, SERCOM_RX_PAD_0, UART_TX_PAD_2);
CatLoRaS76S::~CatLoRaS76S() {}
CatLoRaS76S::CatLoRaS76S()
{
    pinMode(_LED_BUIDIN, OUTPUT);
}
void SERCOM1_Handler(void)
{
    Serial2.IrqHandler();
}
void CatLoRaS76S::macCommand(String comm)
{
    Serial2.print(comm);
    delay(250);
    String msg = Serial2.readString();
#ifdef _DEBUG
    Serial.print("_DEBUG: ");
    Serial.print(comm);
    Serial.println(msg);
#endif
    delay(100);
}
void CatLoRaS76S::begin(uint32_t bundRate)
{
    Wire.begin();
    Serial2.begin(bundRate);
    pinPeripheral(_PIN_TX, PIO_SERCOM);
    pinPeripheral(_PIN_RX, PIO_SERCOM);
    delay(500);
    freqLoraConfig();
    smeHumidity.begin();
}
void CatLoRaS76S::freqLoraConfig(void)
{
    int freq[] = {923200000, 923400000, 922000000, 922200000, 922400000, 922600000, 922800000, 923000000};
    macCommand(String(_MAC_SIP_RESET));
    delay(1000);
    //chanel 0 to 7
    for (int i = 0; i < sizeof freq / sizeof freq[0]; i++)
    {
        macCommand(String(_MAC_SET_CH_FREQ) + " " + i + " " + freq[i]);
    }
    macCommand(String(_MAC_SET_RX2) + " 2 923200000");
    macCommand(String(_MAC_SAVE));
}
void CatLoRaS76S::joinABP(String loraClass, String devEui, String devAddr, String nwksKey, String appsKey)
{
    macCommand(String(_MAC_SET_CLASS) + " " + loraClass);
    macCommand(String(_MAC_SET_DEVEUI) + " " + devEui);
    macCommand(String(_MAC_SET_DEVADDR) + " " + devAddr);
    macCommand(String(_MAC_SET_NWKSKEY) + " " + nwksKey);
    macCommand(String(_MAC_SET_APPSKEY) + " " + appsKey);
    macCommand(String(_MAC_JOIN_ABP));
    macCommand(String(_MAC_SAVE));
}
void CatLoRaS76S::joinOTAA(String loraClass, String devEui, String appEui, String appKey)
{
    macCommand(String(_MAC_SET_CLASS) + " " + loraClass);
    macCommand(String(_MAC_SET_DEVEUI) + " " + devEui);
    macCommand(String(_MAC_SET_APPEUI) + " " + appEui);
    macCommand(String(_MAC_SET_APPKEY) + " " + appKey);
    macCommand(String(_MAC_JOIN_OTAA));
    macCommand(String(_MAC_SAVE));
}
void CatLoRaS76S::transmit(uint8_t port, String payLoad)
{
    String comm = String(_MAC_TX_UCNF) + String(" ") + String(port) + String(" ") + payLoad;
    Serial2.print(comm);
#ifdef _DEBUG
    Serial.print("_DEBUG_TX: ");
    Serial.println(comm);
#endif
    delay(250);
}
String CatLoRaS76S::receive(void)
{
    String msg = Serial2.readString();
    if (msg == "")
    {
        return "";
    }
#ifdef _DEBUG
    Serial.print("_DEBUG_RX: ");
    Serial.println(msg);
#endif
    String rawData = "";
    String checkDL = ">> mac rx ";
    int checkStart = msg.indexOf(checkDL);
    if (checkStart != -1)
    {
        int checkEND = msg.indexOf("\n", checkStart + 1);
        rawData = msg.substring(checkStart + 10, checkEND);
    }
    return rawData;
}
unsigned int CatLoRaS76S::getPortReceive(String dataFromReceive)
{
    String macComm = "";
    if (dataFromReceive != "")
    {
        int checkStart = dataFromReceive.indexOf(' ');
        if (checkStart > -1)
        {
            macComm = dataFromReceive.substring(0, checkStart);
        }
    }
    return (unsigned int)macComm.toInt();
}
String CatLoRaS76S::getPayloadReceive(String dataFromReceive)
{
    String macComm = "";
    if (dataFromReceive != "")
    {
        int checkStart = dataFromReceive.indexOf(' ');
        if (checkStart > -1)
        {
            int checkEND = dataFromReceive.indexOf("\n", checkStart + 1);
            macComm = dataFromReceive.substring(checkStart + 1, checkEND);
        }
    }
    return macComm;
}
double CatLoRaS76S::getTemp()
{
    return smeHumidity.readTemperature();
}
double CatLoRaS76S::getHumi()
{
    return smeHumidity.readHumidity();
}
String CatLoRaS76S::getTempHumiCayenneLPPformat()
{
    String temp = "";
    String humi = "";
    String cayenneLPPformat = "";
    String cayenTemp = "0067"; //3digi (2byte) *0.1(10)
    String cayenHumi = "0168"; //2digi (1byte) *0.5(2)
    temp = uint16_t(this->getTemp() * 10);
    humi = uint16_t(this->getHumi() * 2);
    char tempHex[5];
    char humiHex[3];
    sprintf(tempHex, "%04x", temp.toInt());
    sprintf(humiHex, "%02x", humi.toInt());
    cayenneLPPformat = cayenTemp + tempHex + cayenHumi + humiHex;
    return cayenneLPPformat;
}