void setup() {
  //Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>
 
 
//define the pins used by the LoRa transceiver module
#define ss 10
#define rst 9
#define dio0 2
 
#define LED 5
 
#define RELAY_IO   4
 
#define BAND 433E6    //433E6 for Asia, 866E6 for Europe, 915E6 for North America
 
 
// Initialize variables to get and save LoRa data
int rssi;
int relayswitch;
String status;
String readingID;
 
 
// Replaces placeholder with RElay values
String processor(const String& var)
{
  //Serial.println(var);
  if(var == "STATUS")
  {
    return status;
  }
  else if (var == "RRSI")
  {
    return String(rssi);
  }
  return String();
}
 
void setup() 
{
  Serial.begin(115200);
 
  
  pinMode(RELAY_IO, OUTPUT);
  pinMode(LED, OUTPUT);
  
  int counter;
 
  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0); //setup LoRa transceiver module
 
  while (!LoRa.begin(BAND) && counter < 10) {
    Serial.print(".");
    counter++;
    delay(2000);
  }
  if (counter == 10) 
  {
    // Increment readingID on every new reading
    Serial.println("Starting LoRa failed!"); 
    
  }
    Serial.println("LoRa Initialization OK!"); 
    delay(2000);
 
}
 
 
// Read LoRa packet and get the sensor readings
void loop() 
{
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    Serial.print("Lora packet received: ");
    while (LoRa.available())    // Read packet
  {
    String LoRaData = LoRa.readString();
    Serial.print(LoRaData); 
    
    
    int pos1 = LoRaData.indexOf('&');      
    readingID = LoRaData.substring(0, pos1);                   // Get readingID
    status = LoRaData.substring(pos1 +1, LoRaData.length());   // Get status
 
    relayswitch = status.toInt();  //Converts string to integer
 
    if(relayswitch==1)
    {
      digitalWrite (RELAY_IO, relayswitch);
      digitalWrite (LED, relayswitch); 
      
      Serial.println("Realy in ON");
    }
    
    else if(relayswitch==0)
    {
      digitalWrite (RELAY_IO, relayswitch);
      digitalWrite (LED, relayswitch); 
      Serial.println("Realy in OFF");
    }   
  }
  rssi = LoRa.packetRssi();       // Get RSSI
  Serial.print(" with RSSI ");    
  Serial.println(rssi);
}
}

}

void loop() {
  // put your main code here, to run repeatedly:

}
