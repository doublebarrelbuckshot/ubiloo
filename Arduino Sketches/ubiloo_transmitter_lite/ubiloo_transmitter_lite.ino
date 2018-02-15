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

const int switchPinA = 2;
const int switchPinB = 4;
const int ledGreen = 5;
const int ledRed = 6;

void setup() {
  Serial.begin(57600);
  radio.begin();
  radio.setRetries(15,15);
  radio.startListening();
  radio.printDetails();
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.stopListening();
  
  pinMode(switchPinA, INPUT_PULLUP);
  pinMode(switchPinB, INPUT_PULLUP);
  
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
  
  digitalWrite(ledGreen, HIGH);
  digitalWrite(ledRed, HIGH);
}

void loop() {
  int isOpenA;
  int stallIdA = 1201;

  int isOpenB;
  int stallIdB = 1202;

  radio.stopListening();
  
  int resultA = digitalRead(switchPinA);
  if (resultA == HIGH) {
    isOpenA = 1;
    digitalWrite(ledRed, LOW);
    digitalWrite(ledGreen, HIGH);
  } else {
    isOpenA = -1;
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, LOW);
  }
    
  int resultB = digitalRead(switchPinB);
  if (resultB == HIGH) {
    isOpenB = 1;
  } else {
    isOpenB = -1;
  }

  payload.isOpenA = isOpenA;
  payload.stallIdA = stallIdA;
  
  payload.isOpenB = isOpenB;
  payload.stallIdB = stallIdB;

  //Serial << "isOpenA: " << payload.isOpenA << "  stallIdA: " << stallIdA;
  //Serial << "isOpenB: " << payload.isOpenB << "  stallIdB: " << stallIdB;

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

