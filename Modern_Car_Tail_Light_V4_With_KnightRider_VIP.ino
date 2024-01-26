// HAZI TECH
// Programmed by Hasitha Jayasundara
// Visit my YouTube Channel for more info - http://www.youtube.com/c/HAZITECH?sub_confirmation=1

#include "Arduino.h"
#include <FastLED.h>
#include <EEPROM.h>

#define LED_PIN 2           // LED Strip Signal Connection 
#define ParkSignal 3        // Park Signal Connection
#define BrakeSignal 5       // Brake Signal Connection
#define LeftSignal 7        // Left Blinker Signal Connection
#define RightSignal 9       // Right Blinker Signal Connection
#define ReverseSignal 11    // Reverse Signal Connection
#define ModeButton 12       // Mode Select Button
#define ModeLED 10          // Mode LED

#define NUM_LEDS 60         // Total no of LEDs in the LED strip
#define BlinkerLEDs 15      // No of LEDs for Left/Right Blinker 

int ColourTrig = 2;         // Knight Rider Pattern Colour ; 1-Red, 2-Magenta, 3-Blue, 4-Cyan, 5-Green, 6-Yellow, 7- White
int Park_Brightness = 50;   // Park Light Brightness ; Value Min - 10 , Max - 255

int BlinkerSpeed = 35;      // Turn Signal Running LED Speed. Adjust this to match with your vehicle turn signal speed.
int BlinkerOffDelay = 250;  // Turn Signal Off time. Adjust this to match with your vehicle turn signal speed.

int StartupSpeed = 25;
int ParkDetect = 0;
int mode = 1;
int pres = 0;
int r, g, b;

CRGB leds[NUM_LEDS];


void setup() 
{
EEPROM.update(1,1);       // *IMPORTANT* - Run this code line ONLY for the 1st time. Then comment it and re-upload the code                       
mode = EEPROM.read(1);
FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
pinMode(ParkSignal, INPUT);
pinMode(BrakeSignal, INPUT);
pinMode(LeftSignal, INPUT);
pinMode(RightSignal, INPUT);
pinMode(ReverseSignal, INPUT);
pinMode(ModeLED, OUTPUT);
pinMode(ModeButton, INPUT);
}


void loop() 
{
ButtonScan();

switch (mode) {
case 1:
digitalWrite(ModeLED, LOW);
if(digitalRead(ReverseSignal)==1)
{
if(digitalRead(BrakeSignal)==0) //Reverse Light
{   
Reverse();
}

if(digitalRead(BrakeSignal)==1) //Brake Light
{   
BrakeFull();
}
}

if(ParkDetect == 0)
{
if(digitalRead(ParkSignal)==1) 
{
ParkDetect = 1;
}
}

if(ParkDetect == 2)
{
if(digitalRead(ParkSignal)==0) 
{
ParkDetect = 3;
}
}

if(ParkDetect == 1)
{
ParkON();
ParkDetect = 2;  
}

if(ParkDetect == 3)
{
ParkOFF();
ParkDetect = 0;  
}


if(digitalRead(ParkSignal)==0)
{
if(digitalRead(ReverseSignal)==0)
{
if((digitalRead(BrakeSignal)==0)&&(digitalRead(LeftSignal)==0)&&(digitalRead(RightSignal)==0)) //No Signal
{
AllOff();
}

if((digitalRead(BrakeSignal)==1)&&(digitalRead(LeftSignal)==0)&&(digitalRead(RightSignal)==0)) //Brake Light
{
BrakeFull();
}
    
if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==0)&&(digitalRead(BrakeSignal)==0)) //Left Blinker
{
RightOff();
LeftBlinker();
LeftDim();
delay (BlinkerOffDelay);
}

if((digitalRead(RightSignal)==1)&&(digitalRead(LeftSignal)==0)&&(digitalRead(BrakeSignal)==0)) //Right Blinker
{
LeftOff();
RightBlinker();
RightDim();
delay (BlinkerOffDelay);
}

if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==0)&&(digitalRead(BrakeSignal)==1)) //Left Blinker & Brake
{
LeftDim();
RightFull(); 
LeftBlinker();
LeftDim();
delay (BlinkerOffDelay);
}

if((digitalRead(RightSignal)==1)&&(digitalRead(LeftSignal)==0)&&(digitalRead(BrakeSignal)==1)) //Right Blinker & Brake
{
RightDim();
LeftFull();
RightBlinker();
RightDim();
delay (BlinkerOffDelay);
}

if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==1)&&(digitalRead(BrakeSignal)==0)) //Dual Blinker / Hazard
{
MiddleOff();
DualBlinker();
LeftDim();
RightDim();
delay (BlinkerOffDelay);
}

if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==1)&&(digitalRead(BrakeSignal)==1)) //Dual Blinker / Hazard + Brake
{
LeftDim();
RightDim();
BrakeMiddle();
DualBlinker();
LeftDim();
RightDim();
delay (BlinkerOffDelay);
}
}
}


if(ParkDetect == 2)
{
if(digitalRead(ReverseSignal)==0)
{
if((digitalRead(LeftSignal)==0)&&(digitalRead(RightSignal)==0)&&(digitalRead(BrakeSignal)==0)) //Park Light
{   
ParkFull();
}

if((digitalRead(BrakeSignal)==1)&&(digitalRead(LeftSignal)==0)&&(digitalRead(RightSignal)==0)) //Brake Light
{
BrakeFull();
}
    
if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==0)&&(digitalRead(BrakeSignal)==0)) //Left Blinker
{
LeftDim();
RightLit(); 
LeftBlinker();
LeftDim();
delay (BlinkerOffDelay);
}

if((digitalRead(RightSignal)==1)&&(digitalRead(LeftSignal)==0)&&(digitalRead(BrakeSignal)==0)) //Right Blinker
{
RightDim();
LeftLit();
RightBlinker();
RightDim();
delay (BlinkerOffDelay);
}

if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==0)&&(digitalRead(BrakeSignal)==1)) //Left Blinker & Brake
{
LeftDim();
RightFull(); 
LeftBlinker();
LeftDim();
delay (BlinkerOffDelay);
}

if((digitalRead(RightSignal)==1)&&(digitalRead(LeftSignal)==0)&&(digitalRead(BrakeSignal)==1)) //Right Blinker & Brake
{
RightDim();
LeftFull();
RightBlinker();
RightDim();
delay (BlinkerOffDelay);
}

if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==1)&&(digitalRead(BrakeSignal)==0)) //Dual Blinker / Hazard
{
LeftDim();
RightDim();
ParkMiddle();
DualBlinker();
LeftDim();
RightDim();
delay (BlinkerOffDelay);
}

if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==1)&&(digitalRead(BrakeSignal)==1)) //Dual Blinker / Hazard + Brake
{
LeftDim();
RightDim();
BrakeMiddle();
DualBlinker();
LeftDim();
RightDim();
delay (BlinkerOffDelay);
}
}
}
break;


case 2:
for (int i = 0; i < 3; i++)
{
for (int i = 0; i < NUM_LEDS/2; i++)
{
leds[i] = CRGB(255, 0, 0);
}
FastLED.show();
digitalWrite(ModeLED, HIGH);
delay (75);ButtonScan();

for (int i = 0; i < NUM_LEDS/2; i++)
{
leds[i] = CRGB(0, 0, 0);
}
FastLED.show();
digitalWrite(ModeLED, LOW);
delay (75);ButtonScan();
}

for (int i = 0; i < 3; i++)
{
for (int i = NUM_LEDS/2; i < NUM_LEDS; i++)
{
leds[i] = CRGB(0, 0, 255);
}
FastLED.show();
digitalWrite(ModeLED, HIGH);
delay (75);ButtonScan();

for (int i = NUM_LEDS/2; i < NUM_LEDS; i++)
{
leds[i] = CRGB(0, 0, 0);
}
FastLED.show();
digitalWrite(ModeLED, LOW);
delay (75);ButtonScan();
}
break; 


case 3:
digitalWrite(ModeLED, HIGH);
switch (ColourTrig) 
 {
  case 1:
  r = 255;
  g = b = 0;
  break;
  
  case 2:
  r = b = 255;
  g = 0;
  break;
  
  case 3:
  b = 255;
  r = g = 0;
  break; 

  case 4:
  b = g = 255;
  r = 0;
  break; 

  case 5:
  g = 255;
  r = b = 0;
  break; 

  case 6:
  r = g = 255;
  b = 0;
  break; 

  case 7:
  r = g = b = 255;
  break;
} 

fadeToBlackBy(leds, NUM_LEDS, 20);
int pos = beatsin16(20, 0, NUM_LEDS-1);
leds[pos] += CRGB(r, g, b);
FastLED.show();ButtonScan();  
FastLED.delay(5);  
break;
}
}


























// HAZI TECH
// Programmed by Hasitha Jayasundara
// Visit my YouTube Channel for more info - http://www.youtube.com/c/HAZITECH?sub_confirmation=1

void ParkON()
{
for (int i = 0; i < (NUM_LEDS/2); i++)
  {
    leds[i] = CRGB(30, 0, 0);
    leds[i-1] = CRGB(0, 0, 0);
    leds[(NUM_LEDS-1)-i] = CRGB(30, 0, 0);
    leds[(NUM_LEDS)-i] = CRGB(0, 0, 0);
    FastLED.show();
    delay (StartupSpeed);    
  }
  
  for (int j = ((NUM_LEDS/2)-1); j >= 0; j--)
  {
    leds[j] = CRGB(30, 0, 0);
    leds[(NUM_LEDS/2-1)+((NUM_LEDS/2)-j)] = CRGB(30, 0, 0);
    FastLED.show();
    delay (StartupSpeed);    
  }

  for (int j = ((NUM_LEDS/2)-1); j >= 0; j--)
  {
    leds[j] = CRGB(255, 0, 0);
    leds[(NUM_LEDS/2-1)+((NUM_LEDS/2)-j)] = CRGB(255, 0, 0);
    FastLED.show();
    delay (StartupSpeed);    
  }

  for (int j = 255; j >= Park_Brightness; j--)
  {  
    for (int i = 0; i < NUM_LEDS; i++)
    {
    leds[i] = CRGB(j, 0, 0);
    }
    FastLED.show();  
    delay (5);
  }
}

void ParkOFF()
{
  for (int j = Park_Brightness; j <= 255; j++)
  {  
    for (int i = 0; i < NUM_LEDS; i++)
    {
    leds[i] = CRGB(j, 0, 0);
    }
    FastLED.show();  
    delay (5);
  }

  for (int j = 0; j <= ((NUM_LEDS/2)-1); j++)
  {
    leds[j] = CRGB(255, 0, 0);
    leds[j-1] = CRGB(30, 0, 0);
    leds[(NUM_LEDS-1)-j] = CRGB(255, 0, 0);
    leds[(NUM_LEDS)-j] = CRGB(30, 0, 0);
    FastLED.show();
    delay (StartupSpeed);    
  }

  leds[((NUM_LEDS/2)-1)] = CRGB(30, 0, 0);
  leds[((NUM_LEDS/2)-1)+1] = CRGB(30, 0, 0);
  FastLED.show();
  
  for (int j = 0; j <= ((NUM_LEDS/2)-1); j++)
  {
    leds[j] = CRGB(30, 0, 0);
    leds[j-1] = CRGB(0, 0, 0);
    leds[(NUM_LEDS-1)-j] = CRGB(30, 0, 0);
    leds[(NUM_LEDS)-j] = CRGB(0, 0, 0);
    FastLED.show();
    delay (StartupSpeed);    
  }
}

void AllOff()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB(0, 0, 0);
  }
    FastLED.show();  
}

void LeftOff()
{
  for (int i = 0; i < (NUM_LEDS - BlinkerLEDs); i++)
  {
    leds[i] = CRGB(0, 0, 0);
  }
    FastLED.show();
}

void RightOff()
{
  for (int i = BlinkerLEDs; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB(0, 0, 0);
  }
    FastLED.show();
}

void MiddleOff()
{
  for (int i = BlinkerLEDs; i < (NUM_LEDS - BlinkerLEDs); i++)
  {
    leds[i] = CRGB(0, 0, 0);
  }
    FastLED.show();  
}

void Reverse()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB(255, 255, 255);
  }
    FastLED.show();  
}

void BrakeFull()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB(255, 0, 0);
  }
    FastLED.show();  
}

void BrakeMiddle()
{
  for (int i = BlinkerLEDs; i < (NUM_LEDS - BlinkerLEDs); i++)
  {
    leds[i] = CRGB(255, 0, 0);
  }
    FastLED.show();  
}

void ParkFull()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB(Park_Brightness, 0, 0);
  }
    FastLED.show();
}

void ParkMiddle()
{
  for (int i = BlinkerLEDs; i < (NUM_LEDS - BlinkerLEDs); i++)
  {
    leds[i] = CRGB(Park_Brightness, 0, 0);
  }
    FastLED.show();  
}

void LeftBlinker()
{
  for (int i = (BlinkerLEDs-1); i >= 0; i--)
  {
    leds[i] = CRGB(255, 165, 0);
    FastLED.show();
    delay (BlinkerSpeed);    
  }
}

void LeftDim()
{
  for (int i = 0; i < BlinkerLEDs; i++)
  {
    leds[i] = CRGB(0, 0, 0);
  }
    FastLED.show();
}

void LeftLit()
{
  for (int i = 0; i < (NUM_LEDS - BlinkerLEDs); i++)
  {
    leds[i] = CRGB(Park_Brightness, 0, 0);
  }
    FastLED.show();
}

void LeftFull()
{
  for (int i = 0; i < (NUM_LEDS - BlinkerLEDs); i++)
  {
    leds[i] = CRGB(255, 0, 0);
  }
    FastLED.show();
}

void RightBlinker()
{
  for (int i = (NUM_LEDS - BlinkerLEDs); i < NUM_LEDS; i++)
  {
    leds[i] = CRGB(255, 165, 0);
    FastLED.show();
    delay (BlinkerSpeed);
  }
}

void RightDim()
{
   for (int i = (NUM_LEDS - BlinkerLEDs); i < NUM_LEDS; i++)
  {
    leds[i] = CRGB(0, 0, 0);
  }
    FastLED.show();
}

void RightLit()
{
  for (int i = BlinkerLEDs; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB(Park_Brightness, 0, 0);
  }
    FastLED.show();
}

void RightFull()
{
  for (int i = BlinkerLEDs; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB(255, 0, 0);
  }
    FastLED.show();
}

void DualBlinker()
{
  for (int i = (BlinkerLEDs-1); i >= 0; i--)
  {
    leds[i] = CRGB(255, 165, 0);
    leds[NUM_LEDS-1-i] = CRGB(255, 165, 0);
    FastLED.show();
    delay (BlinkerSpeed);
  }
}

void ButtonScan()
{
if(digitalRead(ModeButton)==1)
   {
    if(pres==0)
   {
    mode++;
    EEPROM.update(1,mode); 
    pres=1;
   }
    }
  else
  {
    pres=0;
   }  

 if(mode == 4)
 {
 mode = 1;
 }
}

// HAZI TECH
// Programmed by Hasitha Jayasundara
// Visit my YouTube Channel for more info - http://www.youtube.com/c/HAZITECH?sub_confirmation=1
