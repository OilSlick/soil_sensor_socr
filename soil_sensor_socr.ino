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

//For LoRa Transmissions
byte localAddress = 06;               // address of this device 
byte webGatewayAddress = 0x31;        // address of web gateway
byte broadcastAddress = 0xFF;         // broadcast address
byte destination = 0x1;               // destination to send to (web gateway
byte msgCount = 0;                    // count of outgoing messages
byte fullpayload[12];                 // Full payload to be sent              
int sizeofFullPayload;                // Size of full payload

//for local device
bool debug = 0;
long previousMillis = 0;              //stores the last time data collected
long intervalMinutes = 60000;        //Polling interval in minutes * 60 * 1000

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
  if ( Serial ) Serial.begin(115200);
  delay(2000);
  //LoRa.setPins(csPin, resetPin, irqPin);
  LoRa.setPins(RFM95_SS, RFM95_RST, RFM95_INT);
  //LoRa.setTxPower(17, PA_OUTPUT_PA_BOOST_PIN);

  if ( Serial ) Serial.println("Initializing LoRa");

  while (!LoRa.begin(433E6)) 
  {
    if ( Serial ) Serial.print(".");
      delay(500);
  }
  if ( Serial ) Serial.println("Setup done.Counting millis...");
}

void loop(void) {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > intervalMinutes)
  {
    previousMillis = currentMillis;


    //scanOneWire();
    //idOneWireDevices();
  
    if ( Serial ) Serial.println("Temp Probe A");
    pollTempProbe(AddrTempProbeA);
    decodeProbeData();
    memset(data, 0, sizeof(data)); //clear array
  
    delay(5000);

    if ( Serial )  
    {
      Serial.println(" ");
      Serial.print("Moisture Probe A: ");
    }
    ValueRawMoistureSensorA = analogRead(moistureSensorA);
    if ( Serial )
    {
      Serial.print(ValueRawMoistureSensorA);
      Serial.print(" (raw) ");
    }
    ValueRawMoistureSensorA = map(ValueRawMoistureSensorA,1023,581,0,100); //map(value, fromLow, fromHigh, toLow, toHigh)
    data[0] = byte(ValueRawMoistureSensorA);
    if ( Serial )
    {
      Serial.print(ValueRawMoistureSensorA);
      Serial.println("%");
      Serial.println(" ");
    }
  
    broadcastdata(data, 1, AddrMoistProbeA);
    memset(data, 0, sizeof(data)); //clear array
  
    if ( Serial )Serial.println("Temp Probe B");
    pollTempProbe(AddrTempProbeB);
    decodeProbeData();
    memset(data, 0, sizeof(data)); //clear array

    if ( Serial )
    {
      Serial.println(" ");
      Serial.print("Moisture Probe B: ");
    }
    ValueRawMoistureSensorB = analogRead(moistureSensorB);
    if ( Serial )
    {
      Serial.print(ValueRawMoistureSensorB);
      Serial.print(" (raw) ");
    }
    ValueRawMoistureSensorB = map(ValueRawMoistureSensorB,1023,581,0,100); //map(value, fromLow, fromHigh, toLow, toHigh)
    data[0] = byte(ValueRawMoistureSensorB);
    if ( Serial )
    {
      Serial.print(ValueRawMoistureSensorB);
      Serial.println("%");
      Serial.println(" ");
    }
    broadcastdata(data, 1, AddrMoistProbeB);
    memset(data, 0, sizeof(data)); //clear array
  
    LoRa.sleep();
   }
}
