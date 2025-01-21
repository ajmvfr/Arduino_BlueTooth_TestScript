#include "Arduino.h"
#include <SoftwareSerial.h>
// #include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    5
// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 8
// Declare our NeoPixel strip object:
// Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

// LED leads connected to PWM pins
const int RED_LED_PIN = 3;
const int GREEN_LED_PIN = 5;
const int BLUE_LED_PIN = 6;

void setLeds(String message);
String getNextNumber(String text, int cursor);
void SetRGB(int red, int green, int blue);
int ScaleToRBG(int initial);

const byte rxPin = 9;
const byte txPin = 8;
SoftwareSerial BTSerial(rxPin, txPin); // RX TX

void setup() {
  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  BTSerial.begin(115200);
  Serial.begin(9600);
  // strip.begin();
  // strip.show(); // Initialize all pixels to 'off'
  pinMode(LED_BUILTIN, OUTPUT);
  SetRGB(0,0,0);
}

// message tokens
const char START_TOKEN = '?';
const char END_TOKEN = ';';
const char DELIMIT_TOKEN = '&';
const int CHAR_TIMEOUT = 20;

bool waitingForStartToken = true;
String messageBuffer = "";

long lastRun = millis();
bool outputValue = false;

void loop() {

  // handle Bluetooth link
  char nextData;
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

  if ((millis() - lastRun) > 1000) {
    lastRun = millis();
    outputValue = !outputValue;
    //Serial.print(outputValue);
    if (outputValue) {
      BTSerial.write("1");
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on
    }
    else {
      BTSerial.write("0");
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED off
    }
  }
  
}

void setLeds(String message){
  int textCursor = 0;
  // int pixel = 0;
  bool rgbOk = true;
  String red = "";
  String green = "";
  String blue = "";

  /*
   * message should be in the format
   * r=xxx&g=xxx&b=xxx
   */
  
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
    // Serial.println("red = " + red);
    // Serial.println("green = " + green);
    // Serial.println("blue = " + blue);
    SetRGB(red.toInt(),green.toInt(),blue.toInt());
    // for(pixel = 0; pixel < LED_COUNT; pixel ++){
    //   strip.setPixelColor(pixel, red.toInt(), green.toInt(), blue.toInt());
    // }
    // strip.show();
  }
}

String getNextNumber(String text, int textCursor){
  String number = "";
  while((text[textCursor] >= '0') && (text[textCursor] <= '9') && (textCursor < text.length())){
    number += text[textCursor];
    textCursor ++;
  }
  return number;
}

void SetRGB(int red, int green, int blue){
  Serial.println("red:" + String(red) + " green:" + String(green) + " blue:" + String(blue));
  analogWrite(RED_LED_PIN, ScaleToRBG(red));
  analogWrite(BLUE_LED_PIN, ScaleToRBG(blue));
  analogWrite(GREEN_LED_PIN, ScaleToRBG(green));
}

int ScaleToRBG(int ColorPercent){

      int scaled = (ColorPercent / 100.0) * 255;

      // Serial.println("Initial Percent:" + String(ColorPercent) + " converted to 256 scale:" + String(scaled));

      return scaled;
}