void scanOneWire()    //function not necessary in production and not used
{
  if ( !ds.search(addr)) 
  {
    Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }
  
  Serial.print("ROM =");
  for( i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return;
  }
  Serial.println();
}

void idOneWireDevices() //function not necessary in production and not used
{
    // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
      return;
  } 
}

void pollTempProbe(byte addr[])
{
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  if ( Serial && debug == 1 )
  {
    Serial.print("  Data = ");
    Serial.print(present, HEX);
    Serial.print(" ");
  }
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    if ( Serial && debug == 1 )
    {
      Serial.print(data[i], HEX);
      Serial.print(" ");
    }
  }
  if ( Serial && debug == 1 ) Serial.println();
  sizeofFullPayload = sizeof(data);
  if ( Serial && debug == 1 )
  {
    Serial.print("Size of payload: ");
    Serial.println(sizeofFullPayload);
  }
  broadcastdata(data, sizeofFullPayload, addr[7]);
}

void decodeProbeData()
{
  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  byte cfg = (data[4] & 0x60);

  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  if ( Serial )
  {
    Serial.print("  Temperature = ");
    Serial.print(celsius);
    Serial.print(" Celsius, ");
    Serial.print(fahrenheit);
    Serial.println(" Fahrenheit"); 
  }
}
