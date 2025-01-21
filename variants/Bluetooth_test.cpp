#include "Arduino.h"
#include <SoftwareSerial.h>

// This is an adaptation of the variants\bluetooth_reg_testing.cpp in this repo
// I removed extra code, proceduralized the code, stwitched some variable to statics in favor better practices  

// LED leads connected to PWM pins
// const int RED_LED_PIN = 3;
// const int GREEN_LED_PIN = 5;
// const int BLUE_LED_PIN = 6;
// const int PULSE_LED_PIN = 13;

#define RED_LED_PIN 3
#define GREEN_LED_PIN 5
#define BLUE_LED_PIN 6
#define PULSE_LED_PIN 13
#define PUSH_BUTTON 2

void setLeds(String message);
String getNextNumber(String text, unsigned int cursor);
void setRGB(int red, int green, int blue);
int scaleToRBG(int initial);
void checkBluetooth();
void sendHeartbeat(int buttonState, int lightSensorState);
int readButton();

const byte rxPin = 9;
const byte txPin = 8;
SoftwareSerial BTSerial(rxPin, txPin); // RX TX

// message tokens
const char START_TOKEN = '?';
const char END_TOKEN = ';';
const char DELIMIT_TOKEN = '&';
const int CHAR_TIMEOUT = 20;


void setup() {
    // define pin modes for tx, rx:
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(BLUE_LED_PIN, OUTPUT);

    pinMode(PUSH_BUTTON, INPUT_PULLUP);

    BTSerial.begin(115200);
    Serial.begin(9600);
    pinMode(PULSE_LED_PIN, OUTPUT);
    setRGB(0,0,0);
}


void loop() {

    static int buttonState = 0; 
    static int lightSensorState = 0; 

    checkBluetooth();

    buttonState = readButton();
 
    sendHeartbeat(buttonState, lightSensorState);
 
}

int readButton(){

    static int buttonState = 0; 

    buttonState = digitalRead(PUSH_BUTTON);

    return buttonState;
}


void checkBluetooth(){

    // handle Bluetooth link
    static char nextData;
    static bool waitingForStartToken = true;
    static String messageBuffer = "";

    if (BTSerial.available()) {
        // check for start of message
        if (waitingForStartToken) {
            do {
                nextData = BTSerial.read();
            } while((nextData != START_TOKEN) && BTSerial.available());
            
            if (nextData == START_TOKEN) {
                // Serial.println("message start");
                waitingForStartToken = false;
            }
        }

        // read command
        if (!waitingForStartToken && BTSerial.available()){
            do {
                nextData = BTSerial.read();
                // Serial.println(nextData);
                messageBuffer += nextData;
            } while((nextData != END_TOKEN) && BTSerial.available());

            // check if message complete
            if (nextData == END_TOKEN) {
                // remove last character
                messageBuffer = messageBuffer.substring(0, messageBuffer.length() - 1);
                // Serial.println("message complete - " + messageBuffer);
                setLeds(messageBuffer);
                messageBuffer = "";
                waitingForStartToken = true;
            }

            // check for char timeout
            if (messageBuffer.length() > CHAR_TIMEOUT) {
                // Serial.println("message data timeout - " + messageBuffer);
                messageBuffer = "";
                waitingForStartToken = true;
            }
        }
    }
}

void sendHeartbeat(int buttonState, int lightSensorState){
    static unsigned long lastRun = millis();
    static bool outputValue = false;


    if ((millis() - lastRun) > 1000) {
        lastRun = millis();
        outputValue = !outputValue;
        //Serial.print(outputValue);
        if (outputValue) {
            BTSerial.write("1");
            if (buttonState == 1){
                BTSerial.write("P");
            }
            //   Serial.write("1");
            // digitalWrite(PULSE_LED_PIN, HIGH);   // turn the LED on
        }
        else {
            BTSerial.write("0");
            if (buttonState == 1){
                BTSerial.write("R");
            }
            //   Serial.write("0");
            // digitalWrite(PULSE_LED_PIN, LOW);   // turn the LED off
        }
    }
}



void setLeds(String message){
    unsigned int textCursor = 0;
    bool rgbOk = true;
    String red = "";
    String green = "";
    String blue = "";

    if (message.startsWith("r=")){
    // correct starting message
        textCursor = 2;
        red = getNextNumber(message, textCursor);
        textCursor += red.length() + 1;
        message=message.substring(textCursor);
    }
    else {
        rgbOk = false;
    }

    if (message.startsWith("g=")){
    // correct starting message
        textCursor = 2;
        green = getNextNumber(message, textCursor);
        textCursor += green.length() + 1;
        message=message.substring(textCursor);
    }
    else {
        rgbOk = false;
    }

    if (message.startsWith("b=")){
        // correct starting message
        textCursor = 2;
        blue = getNextNumber(message, textCursor);
        textCursor += blue.length() + 1;
        message=message.substring(textCursor);
    }
    else {
        rgbOk = false;
    }

    if (rgbOk) {
        setRGB(red.toInt(),green.toInt(),blue.toInt()); 
    }

}

String getNextNumber(String text, unsigned int textCursor){
    String number = "";

    while((text[textCursor] >= '0') && (text[textCursor] <= '9') && (textCursor < text.length())){
        number += text[textCursor];
        textCursor ++;
    }

    return number;
}

void setRGB(int red, int green, int blue){
    Serial.println("red:" + String(red) + " green:" + String(green) + " blue:" + String(blue));
    analogWrite(RED_LED_PIN, scaleToRBG(red));
    analogWrite(BLUE_LED_PIN, scaleToRBG(blue));
    analogWrite(GREEN_LED_PIN, scaleToRBG(green));
}

int scaleToRBG(int ColorPercent){

      int scaled = (ColorPercent / 100.0) * 255;

      // Serial.println("Initial Percent:" + String(ColorPercent) + " converted to 256 scale:" + String(scaled));

      return scaled;
}