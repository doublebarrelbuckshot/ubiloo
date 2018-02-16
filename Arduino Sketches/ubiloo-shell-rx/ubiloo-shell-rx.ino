// For streaming into Serial
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
  radio.printDetails();
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);
  radio.startListening();
}

void loop(void) {
  if (radio.available()) {
    radio.read(&payload, sizeof(payload));
    
  }
}
