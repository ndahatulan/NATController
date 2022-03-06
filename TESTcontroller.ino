#include "NatController.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OUT_1 16
#define OUT_2 17
#define OUT_3 18
#define OUT_4 19

//LCD Declaration
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define pump1Pin OUT_1
#define pump3Pin OUT_3
//NatController pump1(pump1Pin,"PUMP1", PWM1);
//NatController pump3(pump3Pin,"PUMP3", PWM2);
NatController pump1(pump1Pin,"PUMP1");
NatController pump3(pump3Pin,"PUMP3");
unsigned int interval_on = 5; 
unsigned int interval_off = 5;

const unsigned int MAX_MESSAGE_LENGTH = 12;

void setup() {
  Serial.begin(115200);
  delay(1000);
  //pinMode(pump1Pin, OUTPUT);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64, 3C
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  pump1.setSpeed(100);
  pump1.intervalOn = 5;
  pump1.intervalOff = 5;

  pump3.setSpeed(100);
  pump3.intervalOn = 5;
  pump3.intervalOff = 5;
}

void loop() {

  checkSerialInput();
  if(pump1.state) {
    display.clearDisplay();  
    display.setCursor(45,0);  //oled display
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.println("Pump");  
    display.setCursor(30,40);  //oled display
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.println("ON ");
    
  }
  else {
    
    display.clearDisplay();  
    display.setCursor(45,0);  //oled display
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.println("Pump");  
    display.setCursor(30,40);  //oled display
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.println("OFF ");
    display.display();
    
  }
  
  
  pump1.loop();
  pump3.loop();
  Serial.print("S1 Mode: ");
  Serial.print(pump1.mode);
  Serial.print(" S1 State: ");
  Serial.print(pump1.state);
  Serial.print(" S3 Mode: ");
  Serial.print(pump3.mode);
  Serial.print(" S3 State: ");
  Serial.println(pump3.state);
  display.display();
  
  delay(20);

}

void checkSerialInput() {
while (Serial.available() > 0) {
   //Create a place to hold the incoming message
   static char message[MAX_MESSAGE_LENGTH];
   static unsigned int message_pos = 0;

   //Read the next available byte in the serial receive buffer
   char inByte = Serial.read();

   //Message coming in (check not terminating character) and guard for over message size
   if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1) )
   {
     //Add the incoming byte to our message
     message[message_pos] = inByte;
     message_pos++;
   }
   //Full message received...
   else
   {
     //Add null character to string
     message[message_pos] = '\0';
     Serial.println(message);
     int number = atoi(message);
     //Print the message (or do other things)
     if(number == 1) {
      pump1.mode = MODE_ON;
     }
     else if(number == 2) {
      pump1.mode = MODE_OFF;
     }
     else if (number == 5) {
      pump1.mode = MODE_INT;
      pump1.startIntervalMode();
     }
     else if(number == 11) {
      pump3.mode = MODE_ON;
     }
     else if(number == 12) {
      pump3.mode = MODE_OFF;
     }
     else if (number == 15) {
      pump3.mode = MODE_INT;
      pump3.startIntervalMode();
     }
     else {
      pump1.mode = MODE_OFF;
      pump3.mode = MODE_OFF;
     }
     
     Serial.println(pump1.mode);
     Serial.println(pump3.mode);
     //Reset for the next message
     message_pos = 0;
   }
 }
}
