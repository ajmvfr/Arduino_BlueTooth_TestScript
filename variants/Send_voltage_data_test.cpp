#include "Arduino.h"
#include <SoftwareSerial.h>

/* Electronoobs Bluetooth data receive with
 * Android and Arduino. Small example.
 * Remember to disconnect the Rx and Tx pins of the HC-06 when
 * uploading the code
 * 
 * Subscribe: http://www.youtube.com/c/ELECTRONOOBS
 * Tutorial: http://www.electronoobs.com/eng_arduino_tut20.php
 */
//Inputs
int in = A0;

const byte rxPin = 9;
const byte txPin = 8;
SoftwareSerial BTSerial(rxPin, txPin); // RX TX

void setup() {
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  BTSerial.begin(115200);
  
  Serial.begin(9600);   //Set the baud rate of the comunication
  pinMode(in,INPUT);    //Define the pin as input
}

void loop() {
  //Read the analog value
  float val = analogRead(in);
  
  //Divide by 205 to obtain a range from 0 to 5V
  float val2 = val/205;
  
  //Use serial.print to send the data in a "text" format
  Serial.println(val2);
  BTSerial.print(val2);
  delay(400);//Small delay between each data send 
}