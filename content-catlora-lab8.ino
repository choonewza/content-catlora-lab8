/*Downlink from Lora IoT by CAT is 2020012412105920200124143000 (yyyymmddhhmmssyyyymmddhhmmss)
       Start 20200124121059 => 2020-01-24 12:10:59
       Stop 20200124143000 => 2020-01-24 14:30:00
*/

#include <TimeLib.h>
#include <Wire.h>
#include <DS3231.h>

#include "CatLoRaS76S.h"
#include "LedModule.h"
#include "DateTimeLib.h"

#define RED_LED_PIN 13
#define GREEN_LED_PIN 12

#define LORA_ABP_CLASS "C"
#define LORA_DEV_EUI "1296D01174802100"
#define LORA_DEV_ADDR "74802100"
#define LORA_NWKS_KEY "28AED22B7E1516A609CFABF715884F3C"
#define LORA_APPS_KEY "1628AE2B7E15D2A6ABF7CF4F3C158809"

String startLedTime = "1970-01-01 00:00:01";
String stopLedTime = "1970-01-01 00:00:01";

LedModule redLed(RED_LED_PIN);
LedModule greenLed(GREEN_LED_PIN);
uint32_t startLedUnixtime;
uint32_t stopLedUnixtime;

DS3231 clock;
RTCDateTime dt;
uint8_t seconds = 0;

CatLoRaS76S lora;
uint8_t intervalLoraTX = 30;
uint8_t intervalLoraRX = 1;
uint8_t port = 1;
String payload = "00";



void setup() {
  Serial.begin(115200);
  delay(2000);

  // Initialize LED
  redLed.begin();
  greenLed.begin();

  redLed.on();

  // Initialize DS3231
  Serial.println("Initialize DS3231");
  clock.begin();
  if (clock.isReady() && Serial) {
    clock.setDateTime(__DATE__, __TIME__);
  }

  // Initialize LoRa
  Serial.println("-> Lora Setting...");
  lora.begin(115200);
  Serial.println("-> Lora ABP Join...");
  lora.joinABP(String(LORA_ABP_CLASS),
               String(LORA_DEV_EUI),
               String(LORA_DEV_ADDR),
               String(LORA_NWKS_KEY),
               String(LORA_APPS_KEY));

  // Convert datetime setup led to unixtime
  startLedUnixtime = DateTimeLib::unixtime(startLedTime);
  stopLedUnixtime = DateTimeLib::unixtime(stopLedTime);

  // start-up success -> red led turn-on;

  redLed.off();
  Serial.println("-> Ready Go.");
}

void loop() {
  dt = clock.getDateTime();

  if (dt.second != seconds) {
    seconds = dt.second;

    // ----- Show current datetime on terminal
    String datetime = String(clock.dateFormat("Y-m-d H:i:s", dt));
    Serial.println(datetime);

    // ----- Convert current datetime to unixtime -----
    uint32_t utNow = DateTimeLib::unixtime(dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second);

    // ----- Check datetime for action -----
    if (utNow >= startLedUnixtime && utNow <= stopLedUnixtime) {
      greenLed.on();
    } else {
      greenLed.off();
    }

    // ----- LoRa TX -----
    if (seconds % intervalLoraTX == 0) {
      loraTransmit();
    }

    // ----- LoRa RX -----
    if (seconds % intervalLoraRX == 0) {
      loraReceive();
    }
  }
}

void loraTransmit() {
  lora.transmit(port, payload);
}

void loraReceive() {
  String receive = lora.receive();
  if (receive != "") {
    unsigned int rxPort = lora.getPortReceive(receive);
    String rxPayload = lora.getPayloadReceive(receive);
    if (rxPort != 0 && rxPayload != "") {
      Serial.println("-----RX From CAT LORA-----");
      Serial.println(String("Port = ") + rxPort);
      Serial.println(String("Payload = ") + rxPayload);

      /*Downlink from Lora IoT by CAT is 2020012412105920200124143000 (yyyymmddhhmmssyyyymmddhhmmss)
        Start 20200124121059 => 2020-01-24 12:10:59
        Stop 20200124143000 => 2020-01-24 14:30:00
        rxPayload = "2020012412105920200124143000";
      */

      //new start datetime
      uint16_t sYear = (uint16_t) rxPayload.substring(0, 4).toInt();
      uint8_t sMonth = (uint8_t) rxPayload.substring(4, 6).toInt();
      uint8_t sDay = (uint8_t) rxPayload.substring(6, 8).toInt();
      uint8_t sHour = (uint8_t) rxPayload.substring(8, 10).toInt();
      uint8_t sMinute = (uint8_t) rxPayload.substring(10, 12).toInt();
      uint8_t sSecond = (uint8_t) rxPayload.substring(12, 14).toInt();

      //new stop datetime
      uint16_t eYear = (uint16_t) rxPayload.substring(14, 18).toInt();
      uint8_t eMonth = (uint8_t) rxPayload.substring(18, 20).toInt();
      uint8_t eDay = (uint8_t) rxPayload.substring(20, 22).toInt();
      uint8_t eHour = (uint8_t) rxPayload.substring(22, 24).toInt();
      uint8_t eMinute = (uint8_t) rxPayload.substring(24, 26).toInt();
      uint8_t eSecond = (uint8_t) rxPayload.substring(26, 28).toInt();

      //set startLedUnixtime and stopLedUnixtime
      startLedUnixtime = DateTimeLib::unixtime(sYear, sMonth, sDay, sHour, sMinute, sSecond);
      stopLedUnixtime = DateTimeLib::unixtime(eYear, eMonth, eDay, eHour, eMinute, eSecond);

      //set datetime format yyyy-mm-dd HH:mm:ss
      char startLedDateTime[32];
      char stopLedDateTime[32];
      sprintf(startLedDateTime, "%04d-%02d-%02d %02d:%02d:%02d", sYear, sMonth, sDay, sHour, sMinute, sSecond);
      sprintf(stopLedDateTime, "%04d-%02d-%02d %02d:%02d:%02d", eYear, eMonth, eDay, eHour, eMinute, eSecond);

      //Show new datetime
      Serial.println(String("New Start LED : ") + startLedDateTime);
      Serial.println(String("New Stop LED : ") + stopLedDateTime);
    }
  }
}
