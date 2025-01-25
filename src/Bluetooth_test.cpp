#include "Arduino.h"
#include <SoftwareSerial.h>

// This is an adaptation of the variants\bluetooth_reg_testing.cpp in this repo
// I removed extra code, proceduralized the code, stwitched some variable to statics in favor better practices  

// LED leads connected to PWM pins
// const int RED_LED_PIN = 3;
// const int GREEN_LED_PIN = 5;
// const int BLUE_LED_PIN = 6;
// const int PULSE_LED_PIN = 13;

const int RED_LED_PIN = 3;
const int GREEN_LED_PIN = 5;
const int BLUE_LED_PIN = 6;
const int PULSE_LED_PIN = 13;
const int PUSH_BUTTON = 2;
const int ldr_pin = 7;
const byte rxPin = 9;
const byte txPin = 8;

void setLeds(String message);
String getNextNumber(String text, unsigned int cursor);
void setRGB(int red, int green, int blue);
int scaleToRBG(int initial);
void checkBluetooth();
void sendHeartbeat(int buttonState, int lightSensorState);
int readButton();
void turnOnLed(int ledOnOff);

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
    pinMode(ldr_pin,INPUT);

    pinMode(PUSH_BUTTON, INPUT_PULLUP);

    BTSerial.begin(115200);
    Serial.begin(9600);
    pinMode(PULSE_LED_PIN, OUTPUT);
    setRGB(0,0,0);
    Serial.println(F(__FILE__ " " __DATE__ " " __TIME__));
}


void loop() {

    static int ledOnOff = 0; 
    static int lightSensorState = 0; 

    checkBluetooth();
    ledOnOff = readButton();
    turnOnLed(ledOnOff);
    lightSensorState = digitalRead(ldr_pin);
    sendHeartbeat(ledOnOff, lightSensorState);
 
}


void turnOnLed(int ledOnOff){

    if (ledOnOff){
         digitalWrite(PULSE_LED_PIN, HIGH);
    }
    else{
        digitalWrite(PULSE_LED_PIN, LOW);
    }    
}

int readButton(){

    static int SwitchOnOff = 0;
    static unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
    static unsigned long debounceDelay = 50UL;

    int reading = digitalRead(PUSH_BUTTON);
    static int previouseReading = LOW;


    if (reading == HIGH && previouseReading == LOW && (millis() - lastDebounceTime) > debounceDelay) {
        // whatever the reading is at, it's been there for longer
        // than the debounce delay, so take it as the actual current state:
        if (SwitchOnOff == HIGH)
            SwitchOnOff = LOW;
        else
            SwitchOnOff = HIGH;
        
        lastDebounceTime = millis();
    }
  
    previouseReading = reading;

    return SwitchOnOff;
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

    String uploadString;

    if ((millis() - lastRun) > 500) {
        lastRun = millis();
        outputValue = !outputValue;
        //Serial.print(outputValue);
        uploadString = "?" + String(outputValue) + "|" + String(buttonState) + "|" + String(lightSensorState) + ";";
        Serial.println(uploadString);
        BTSerial.println(uploadString);
        // if (outputValue) {
        //     BTSerial.write("1");

        // }
        // else {
        //     BTSerial.write("0");
        // }
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