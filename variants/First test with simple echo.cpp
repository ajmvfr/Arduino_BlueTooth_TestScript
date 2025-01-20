#include "Arduino.h"
#include <SoftwareSerial.h>

const byte rxPin = 9;
const byte txPin = 8;
SoftwareSerial BTSerial(rxPin, txPin); // RX TX

void setup() {
  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  BTSerial.begin(115200);
  Serial.begin(9600);
}

String messageBuffer = "";
String message = "";

void loop() {
  
  while (BTSerial.available() > 0) {
    char data = (char) BTSerial.read();
    messageBuffer += data;
    if (data == ';'){
      message = messageBuffer;
      messageBuffer = "";
      message = "You Sent : " + message + " from phone";
      Serial.print(message); // send to serial monitor
      BTSerial.print(message); // send back to bluetooth terminal
    }
  }

  while (Serial.available() > 0) {
    char data = (char) Serial.read();
    messageBuffer += data;
    if (data == ';'){
      message = messageBuffer;
      messageBuffer = "";
      message = "You Sent : " + message + " from PC";
      BTSerial.print(message); // send to serial monitor
      Serial.print(message); // send back to bluetooth terminal
    }
  }

}

