/*
 *Board: Adafruit Feather M0 RFM9x LoRa 433MHz
 *Might be a good read: https://learn.adafruit.com/adafruit-feather-m0-bluefruit-le/adapting-sketches-to-m0
 *Temp probes: DS18B20 https://www.adafruit.com/product/381
 *Moisture probes: FC-28 (MH Series) resistance sensor
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
byte fullpayload[12];                 // Full payload to be sent              
int sizeofFullPayload;                // Size of full payload

bool debug = 1;

//For temp probes
byte AddrTempProbeA[8] = {0x28, 0x1F, 0x2D, 0x45, 0x92, 0x5, 0x2, 0x30};
byte AddrTempProbeB[8] = {0x28, 0xEB, 0x9C, 0x45, 0x92, 0x10, 0x2, 0xF};

//For moisture probes
byte AddrMoistProbeA = 0x1;         // ID for moisture probe A
byte AddrMoistProbeB = 0x2;         // ID for moisture probe B
int moistureSensorA = A1;             // Soil Sensor input at Analog PIN A1
int moistureSensorB = A2;             // Soil Sensor input at Analog PIN A2
int ValueRawMoistureSensorA;          // Raw value returned from moisture sensor A
int ValueRawMoistureSensorB;          // Raw value returned from moisture sensor B

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

  Serial.println("Temp Probe A");
  pollTempProbe(AddrTempProbeA);
  decodeProbeData();
  memset(data, 0, sizeof(data)); //clear array
  
  delay(5000);

  Serial.println(" ");
  Serial.print("Moisture Probe A: ");
  ValueRawMoistureSensorA = analogRead(moistureSensorA);
  Serial.print(ValueRawMoistureSensorA);
  Serial.print(" (raw) ");
  ValueRawMoistureSensorA = map(ValueRawMoistureSensorA,1023,581,0,100); //map(value, fromLow, fromHigh, toLow, toHigh)
  data[0] = byte(ValueRawMoistureSensorA);
  Serial.print(ValueRawMoistureSensorA);
  Serial.println("%");
  Serial.println(" ");
  broadcastdata(data, 1, AddrMoistProbeA);
  memset(data, 0, sizeof(data)); //clear array
  
  Serial.println("Temp Probe B");
  pollTempProbe(AddrTempProbeB);
  decodeProbeData();
  memset(data, 0, sizeof(data)); //clear array

  Serial.println(" ");
  Serial.print("Moisture Probe B: ");
  ValueRawMoistureSensorB = analogRead(moistureSensorB);
  Serial.print(ValueRawMoistureSensorB);
  Serial.print(" (raw) ");
  ValueRawMoistureSensorB = map(ValueRawMoistureSensorB,1023,581,0,100); //map(value, fromLow, fromHigh, toLow, toHigh)
  data[0] = byte(ValueRawMoistureSensorB);
  Serial.print(ValueRawMoistureSensorB);
  Serial.println("%");
  Serial.println(" ");
  broadcastdata(data, 1, AddrMoistProbeB);
  memset(data, 0, sizeof(data)); //clear array
  
  //LoRa.sleep();
  //delay(1000*60*30); //delay 30 minutes
  delay(1000);
}
