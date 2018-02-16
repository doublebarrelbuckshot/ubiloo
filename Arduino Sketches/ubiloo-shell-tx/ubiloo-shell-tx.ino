template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
RF24 radio(9,10);

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

struct RADIO_PAYLOAD {

};

RADIO_PAYLOAD payload;

void setup() {
  Serial.println("BOOTING");
  Serial.begin(57600);
  
  radio.begin();
  radio.setRetries(15,15);
  radio.startListening();
  radio.printDetails();
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.stopListening();
}

void loop() {
  radio.stopListening();

  sendPayload();
  radio.startListening();

  delay(200);
}

/**
 * Sends the payload.
 * Copies the payload into a buffer and sends that buffer.
 * This buffer is then reconstructed into a RADIO_PAYLOAD on
 * the Receiver side.
 */
void sendPayload() {
  //Serial.println("Transmitting: ");
  char buff[sizeof(payload)];
  memcpy(buff, &payload, sizeof(payload));
  bool ok = radio.write(&buff, sizeof(payload));
  if (ok) {
    Serial.println("Payload Sent");
  } else {
    Serial.println("Send Failed");
  }
}

