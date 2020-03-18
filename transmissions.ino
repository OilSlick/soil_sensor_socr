void broadcastData(byte data[], int sizeofFullPayload, byte ProbeAddr) {
  delay(200);                           // may be needed?
  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(sizeofFullPayload);        // add payload length
  LoRa.write(ProbeAddr);                // Probe Address
  LoRa.write(data, sizeofFullPayload);  // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID

//  if ( debug == true && Serial ) displayDebug("broadcastMessage()");
  digitalWrite(RFM95_SS, HIGH);
}
