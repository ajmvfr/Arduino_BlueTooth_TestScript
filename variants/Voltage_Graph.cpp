#include "Arduino.h"
#include <SoftwareSerial.h>

/* Electronoobs Bluetooth data receive with
 * Android and Arduino. Small example.
 * Remember to disconnect the Rx and Tx pins of the HC-06 when
 * uploading the code
 * 
 * Subscribe: http://www.youtube.com/c/ELECTRONOOBS
 * Tutorial: http://www.electronoobs.com/eng_arduino_tut20_1.php
 */
//Inputs
int in = A0;

const byte rxPin = 9;
const byte txPin = 8;
SoftwareSerial BTSerial(rxPin, txPin); // RX TX

void setup() {
  Serial.begin(9600);
  pinMode(in,INPUT);
  
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  BTSerial.begin(115200);
}

void loop() {
  //Map the value from 10 bits to 8 bits:
  
  byte val = map(analogRead(in),0,1024.0,0,255);    //Byte = 8 bits = 2 to the 8 = 255  
  Serial.write(val);                                //Serial.write will send only one byte at a time 
  BTSerial.write(val);
  delay(400);                                        //Small delay between each data send  
}