#include "heltec.h"
//#include "images.h"

#define BAND   915E6

#define trig 12
#define echo 13
#define buttonPin 38

int buttonState = 0;

unsigned int counter = 0;
String rssi = "RSSI --";
String packSize = "--";
String packet ;


/////////////////////////////////////////////////////////////
//void logo()
//{
  //Heltec.display->clear();
  //Heltec.display->drawXbm(0, 5, logo_width, logo_height, logo_bits);
  //Heltec.display->display();
//}
/////////////////////////////////////////////////////////////


void setup() {

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(buttonPin, INPUT);

  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);

  Heltec.display->init();
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_10);
  //logo();
  delay(1500);
  Heltec.display->clear();

  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->display();
  delay(1000);
}

void loop() {
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);

  Heltec.display->drawString(0, 0, "Sending packet: ");
  Heltec.display->drawString(90, 0, String(counter));
  Heltec.display->display();

  delay(2000);

  if (buttonPress() || readHCSR04() >= 15) {
    // send packet
    LoRa.beginPacket();
    LoRa.setTxPower(14, RF_PACONFIG_PASELECT_PABOOST);
    LoRa.print("Change Actuator #");
    LoRa.print(counter);
    LoRa.endPacket();

    counter++;
    digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);                       // wait for a second
  }
  delay(10000);
}


/////////////////////////////////////////////////////////////
bool buttonPress() {
  int start_time = millis();
  while (millis() - start_time <= 5000) {
    Heltec.display->clear();
    Heltec.display->drawString(0, 0, "Checking Button");
    Heltec.display->display();
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
      Heltec.display->clear();
      Heltec.display->drawString(0, 0, "Button Pressed");
            Heltec.display->drawString(0, 11, "Packet Sending");
      Heltec.display->display();
      delay(1000);
      return true;
    }
  }
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "No Button Press");
  Heltec.display->display();
  return false;
}


/////////////////////////////////////////////////////////////
int readHCSR04() {
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);

  Heltec.display->drawString(0, 0, "Entering Function");
  Heltec.display->display();

  delay(2000);

  int average = 0;

  //average 5 values over 10sec
  for (int i = 1; i <= 5; i++) {

    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    int t = pulseIn(echo, HIGH); //measure how long it takes for pulse to reach echo
    int distance = t / 58; //calibration on Arduino for HC-SR04 into cm

    Heltec.display->clear();
    Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
    Heltec.display->setFont(ArialMT_Plain_10);

    Heltec.display->drawString(0, 0, "Distance ");
    Heltec.display->drawString(43, 0, String(i));
    Heltec.display->drawString(49, 0, "/5: ");
    Heltec.display->drawString(65, 0, String(distance)); //print distance with calibration
    Heltec.display->drawString(80, 0, "cm");
    Heltec.display->display();

    average = average + distance;
    delay(1000);
  }
  average = average / 5;

  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);

  Heltec.display->drawString(0, 20, "Averaged distance: ");
  Heltec.display->drawString(93, 20, String(average));
  Heltec.display->drawString(107, 20, " cm");
  Heltec.display->display();
  delay(2000);
  return average;
}
