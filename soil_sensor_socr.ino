/*
 *Board: Adafruit Feather M0 RFM9x LoRa 433MHz
 *Might be a good read: https://learn.adafruit.com/adafruit-feather-m0-bluefruit-le/adapting-sketches-to-m0
 *Libraries MutichannelGasSensor.h and MutichannelGasSensor.cpp both adapted for Adafruit SAMD M0 boards (serial vs serialusb)
*/
#include <OneWire.h>

// OneWire DS18S20, DS18B20, DS1822 Temperature Example
//
// http://www.pjrc.com/teensy/td_libs_OneWire.html
//
// The DallasTemperature library can do all this work for you!
// https://github.com/milesburton/Arduino-Temperature-Control-Library

OneWire  ds(5);  // on pin 10 (a 4.7K resistor is necessary)
byte i;
byte present = 0;
byte type_s;
byte data[12];
byte addr[8];
float celsius, fahrenheit;


//For LoRa 
#include <LoRa.h>                     //Needed for LoRa
#define RFM95_SS 8                    //The CS pin (#8) does not have a pullup built in so be sure to set this pin HIGH when not using the radio!
#define RFM95_RST 4
#define RFM95_INT 3

byte localAddress = 06;               // address of this device 
byte webGatewayAddress = 0x31;        // address of web gateway
byte broadcastAddress = 0xFF;         // broadcast address
byte destination = 0x1;               // destination to send to (web gateway
byte msgCount = 0;                    // count of outgoing messages

byte fullpayload[12];                 
int sizeofFullPayload;

bool debug = 1;
byte AddrTempProbeA[8] = {0x28, 0x1F, 0x2D, 0x45, 0x92, 0x5, 0x2, 0x30};
byte AddrTempProbeB[8] = {0x28, 0xEB, 0x9C, 0x45, 0x92, 0x10, 0x2, 0xF};

void setup(void) {
  Serial.begin(115200);
  //LoRa.setPins(csPin, resetPin, irqPin);
  LoRa.setPins(RFM95_SS, RFM95_RST, RFM95_INT);
  //LoRa.setTxPower(17, PA_OUTPUT_PA_BOOST_PIN);

  if ( Serial ) Serial.println("Initializing LoRa");

  while (!LoRa.begin(433E6)) 
  {
    if ( Serial ) Serial.print(".");
      delay(500);
  }

  
}

void loop(void) {

  //scanOneWire();
  //idOneWireDevices();

  Serial.println("Probe A");
  pollTempProbe(AddrTempProbeA);
  decodeProbeData();
  
  delay(5000);
  
  Serial.println("Probe B");
  pollTempProbe(AddrTempProbeB);
  decodeProbeData();
  
  //LoRa.sleep();
  delay(15000);
}
