/*
  This is a simple example show the Heltec.LoRa recived data in OLED.

  The onboard OLED display is SSD1306 driver and I2C interface. In order to make the
  OLED correctly operation, you should output a high-low-high(1-0-1) signal by soft-
  ware to OLED's reset pin, the low-level signal at least 5ms.

  OLED pins to ESP32 GPIOs via this connecthin:
  OLED_SDA -- GPIO4
  OLED_SCL -- GPIO15
  OLED_RST -- GPIO16

*/
#include "heltec.h"
//#include "images.h"

#define BAND    915E6  //you can set band here directly,e.g. 868E6,915E6

const int IN1_PIN = 12; // pin connected to the IN1 pin L298N
const int IN2_PIN = 13; // pin connected to the IN2 pin L298N
int currentPosition = 0;

String rssi = "RSSI --";
String packSize = "--";
String packet ;

//void logo() {
 // Heltec.display->clear();
  //Heltec.display->drawXbm(0, 5, logo_width, logo_height, logo_bits);
  //Heltec.display->display();
//}

void LoRaData() {
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0 , 15 , "Received " + packSize + " bytes");
  Heltec.display->drawStringMaxWidth(0 , 26 , 128, packet);
  Heltec.display->drawString(0, 0, rssi);
  Heltec.display->display();
}

void cbk(int packetSize) {
  packet = "";
  packSize = String(packetSize, DEC);
  for (int i = 0; i < packetSize; i++) {
    packet += (char) LoRa.read();
  }
  rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ;
  LoRaData();
}

void setup() {

  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);

  //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);

  Heltec.display->init();
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_10);
  //logo();
  delay(1500);
  Heltec.display->clear();

  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->drawString(0, 10, "Wait for incoming data...");
  Heltec.display->display();
  delay(1000);
  //LoRa.onReceive(cbk);
  LoRa.receive();
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    //  Heltec.display->clear();
    //
    //  Heltec.display->drawString(0, 0, "Packet Received");
    //  Heltec.display->display();
    //  delay(1000);
    cbk(packetSize);
    delay(2000);
    currentPosition = moveActuator(currentPosition);
  }
  delay(10);
}

int moveActuator(int currentPosition){

  if (currentPosition == 0) {
    Heltec.display->clear();
    Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
    Heltec.display->setFont(ArialMT_Plain_10);

    Heltec.display->drawString(0, 0, "Extending Actuator...");
    Heltec.display->display();
    delay(1000);

    digitalWrite(IN1_PIN, LOW);
    digitalWrite(IN2_PIN, HIGH);
    delay(5000); // actuator will stop retracting automatically when reaching the limit
    return 1;
  }
  else if (currentPosition == 1) {
    Heltec.display->clear();
    Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
    Heltec.display->setFont(ArialMT_Plain_10);

    Heltec.display->drawString(0, 0, "Retracting Actuator...");
    delay(1000);

    digitalWrite(IN1_PIN, HIGH);
    digitalWrite(IN2_PIN, LOW);
    delay(5000); // actuator will stop extending automatically when reaching the limit
    return 0;
  }
  delay(10000);
}
