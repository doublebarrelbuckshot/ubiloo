// For streaming into Serial
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; } 

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
RF24 radio(9,10);

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

struct RADIO_PAYLOAD {
  int stallIdA;
  int isOpenA;
  
  int stallIdB;
  int isOpenB;
};

RADIO_PAYLOAD payload;

const int ledGreen = 5;
const int ledRed = 6;

void setup() {
  Serial.begin(57600);
  Serial.println("BOOTING");

  radio.begin();
  radio.setRetries(15,15);
  radio.printDetails();
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);
  radio.startListening();
  
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
}

void loop(void) {
  if (radio.available()) {
    radio.read(&payload, sizeof(payload));
    if (payload.isOpenA == 1) {
      digitalWrite(ledRed, LOW);
      digitalWrite(ledGreen, HIGH);
    } else if (payload.isOpenA == -1) {
      digitalWrite(ledRed, HIGH);
      digitalWrite(ledGreen, LOW);
    } else {
      digitalWrite(ledRed, HIGH);
      digitalWrite(ledGreen, HIGH);
    }
    
    Serial << payload.stallIdA << "=" << payload.isOpenA << ":" << payload.stallIdB << "=" << payload.isOpenB;
  }
}

