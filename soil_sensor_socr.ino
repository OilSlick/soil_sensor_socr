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

#define OneWirePin 5
OneWire  ds(OneWirePin);
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
bool debug = 1;
long previousMillis = 0;              //stores the last time data collected
long intervalMinutes = 300000;        //Polling interval in minutes * 60 * 1000
#define VBATPIN A7
   
//For temp probes
byte AddrTempProbeA[8] = {0x28, 0x1F, 0x2D, 0x45, 0x92, 0x5, 0x2, 0x30};
byte AddrTempProbeB[8] = {0x28, 0xEB, 0x9C, 0x45, 0x92, 0x10, 0x2, 0xF};

//For moisture probes
int moisturePin;                      // Empty byte to be populated by checkMoisture()
byte AddrMoistProbe;                  // Empty byte to be populated by checkMoisture()
byte AddrMoistProbeA = 0x1;           // ID for moisture probe A
byte AddrMoistProbeB = 0x2;           // ID for moisture probe B
int moisturePinA = A1;                // Soil Sensor input at Analog PIN A1
int moisturePinB = A2;                // Soil Sensor input at Analog PIN A2
int ValueRawMoistureSensorA;          // Raw value returned from moisture sensor A
int ValueRawMoistureSensorB;          // Raw value returned from moisture sensor B

void setup(void) {
  if ( debug == 1 ) intervalMinutes = 30000; //if debug, use 30-seccond polling intervals   
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
  if ( Serial ) Serial.println("Setup done. Counting electric sheep...");
}

void loop(void) {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > intervalMinutes)
  {
    previousMillis = currentMillis;

    //scanOneWire();            //function not necessary in production and not used
    //idOneWireDevices();       //function not necessary in production and not used
  
    if ( Serial ) Serial.println("Temp Probe A");
    pollTempProbe(AddrTempProbeA);
    if ( Serial ) decodeProbeData();

    delay(5000);
    checkMoisture('A');
    
    delay(5000); //dealy 5 seconds to allow full transmission of data from this unit to web gateway, to web
  
    if ( Serial ) Serial.println("Temp Probe B");
    pollTempProbe(AddrTempProbeB);
    if ( Serial ) decodeProbeData();

    delay(5000); //dealy 5 seconds to allow full transmission of data from this unit to web gateway, to web

    checkMoisture('B');
    LoRa.sleep();

    //check battery level
    if ( Serial && debug == 1 ) checkBatt();
    if ( Serial ) Serial.println("Counting electric sheep...");
   }
}

void checkBatt()
{
  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  Serial.print("VBat: " ); Serial.println(measuredvbat);
}
void checkMoisture(int probeID)
{
  if (probeID == 'A')             //If moisture probe A
  {
    AddrMoistProbe = AddrMoistProbeA;
    moisturePin = moisturePinA;
  }
  else if (probeID == 'B')         //If moisture probe B
  {
    AddrMoistProbe = AddrMoistProbeB;
    moisturePin = moisturePinB;
  }
  if ( Serial )
    {
      Serial.println("===");
      Serial.print("probeID: ");
      Serial.println(probeID);
      Serial.print("AddrMoistProbe: ");
      Serial.println(AddrMoistProbe, HEX);
      Serial.print("moisturePin: ");
      Serial.println(moisturePin);
    }
  int ValueRawMoistureSensor = analogRead(moisturePin);

  //encode moisture data and add it to the last two bytes of data[] adapted from: https://www.thethingsnetwork.org/docs/devices/bytes.html
  data[0] = highByte(ValueRawMoistureSensor);
  data[1] = lowByte(ValueRawMoistureSensor);

  broadcastdata(data, 2, AddrMoistProbe);
  
  if ( Serial )  
    {
      Serial.println(" ");
      Serial.print("Moisture Probe: ");
      if (probeID == 'A') Serial.print("A ");
      if (probeID == 'B') Serial.print("B ");
      Serial.print(ValueRawMoistureSensor);
      Serial.print(" (raw) ");
      ValueRawMoistureSensor = map(ValueRawMoistureSensor,1023,581,0,100); //map(value, fromLow, fromHigh, toLow, toHigh)
      Serial.print(ValueRawMoistureSensor);
      Serial.println("%");
      Serial.println(" ");
      Serial.println("===");
    }
}
