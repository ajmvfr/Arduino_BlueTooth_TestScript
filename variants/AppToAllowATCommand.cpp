#include "Arduino.h"
#include <SoftwareSerial.h>

const byte rxPin = 9;
const byte txPin = 8;
const int led_pin = 13;
SoftwareSerial BTSerial(rxPin, txPin); // RX TX

char val;  //Store received variables
char val2;  //Store received variables
int ret; //store return value

void setup() {
  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  pinMode(led_pin,OUTPUT);
  BTSerial.begin(115200);
  Serial.begin(9600);
}

void loop() {
  //If the serial port receives the data, it outputs it to the Bluetooth serial port.
  if (Serial.available()) {
    val = Serial.read();
    BTSerial.print(val);    
  }

  //If the Bluetooth module data is received, it is output to the screen.
  if (BTSerial.available()) {
    val = BTSerial.read();
    Serial.print(val);
  }
}

