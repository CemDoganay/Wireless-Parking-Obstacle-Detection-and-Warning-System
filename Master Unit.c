#include "Wire.h"
#include "LCD.h"
#include "LiquidCrystal_I2C.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Arduino.h>
#include <TM1637Display.h>

#define CLK_NW 11
#define DIO_NW 10

#define CLK_NE 9
#define DIO_NE 8

#define CLK_SW 7
#define DIO_SW 6

#define CLK_SE 5
#define DIO_SE 4

#define button A8
#define battery_input A0

RF24 radio(12, 13);
const byte address[6] = "00001";

TM1637Display display_NW(CLK_NW, DIO_NW);
TM1637Display display_NE(CLK_NE, DIO_NE);
TM1637Display display_SW(CLK_SW, DIO_SW);
TM1637Display display_SE(CLK_SE, DIO_SE);

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7);

void setup() 
{
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.startListening();
  
  lcd.begin(20,4);
  lcd.setBacklightPin(3,POSITIVE); // BL, BL_POL
  lcd.setBacklight(HIGH);
  
  Serial.begin(9600); 
}

void loop() {
 display_NW.setBrightness(0x0f);
 display_NE.setBrightness(0x0f);
 display_SW.setBrightness(0x0f);
 display_SE.setBrightness(0x0f);

 int sensorValue  = analogRead(button);
 int mode= sensorValue * (5.0 / 1023.0);
 delay(200);

 int x = analogRead(battery_input);
 int battery = x * (5.0 / 1023.0);

 if(mode>4)
 {
    display_NW.clear();
    display_NE.clear();
    display_SW.clear();
    display_SE.clear();
    
    lcd.setCursor(0,0);
    lcd.print("                    ");
    
    lcd.setCursor(2,0);
    lcd.print("Battery Status");

    if(battery<2.5)
    {
    lcd.setCursor(0,3);
    lcd.print("                    ");
   
    lcd.setCursor(5,3);
    lcd.print("Main:");
    
    lcd.setCursor(11,3);
    lcd.print("Low");
    }
    
    else
    {
    lcd.setCursor(0,3);
    lcd.print("                    ");
    
    lcd.setCursor(5,3);
    lcd.print("Main:");
    
    lcd.setCursor(11,3);
    lcd.print("Good");
    }
    
   
   if (radio.available()) 
   {
    byte Array[6];
    radio.read(&Array, sizeof(Array));
    
    if(Array[0] == 1)
    {
    lcd.setCursor(0,1);
    lcd.print("          ");
    
    lcd.setCursor(0,1);
    lcd.print("NW:");
      
      if(Array[6] == 1)
      {
      lcd.setCursor(4,1);
      lcd.print("Good");
      }
      
      else if(Array[6] == 0)
      {
      lcd.setCursor(4,1);
      lcd.print("Low");
      }
     }
     
    else if(Array[0] == 2)
    {
    lcd.setCursor(10,1);
    lcd.print("          ");
    
    lcd.setCursor(10,1);
    lcd.print("NE:");
      
      if(Array[6] == 1)
      {
      lcd.setCursor(14,1);
      lcd.print("Good");
      }
     
      else if(Array[6] == 0)
      {
      lcd.setCursor(14,1);
      lcd.print("Low");
      }
     }
     
    else if(Array[0] == 3)
    {
    lcd.setCursor(10,2);
    lcd.print("          ");
    lcd.setCursor(10,2);
    lcd.print("SE:");
      if(Array[6] == 1)
      {
      lcd.setCursor(14,2);
      lcd.print("Good");
      }
      else if(Array[6] == 0)
      {
      lcd.setCursor(14,2);
      lcd.print("Low");
      }
     }
    
    else if(Array[0] == 4)
    {
    lcd.setCursor(0,2);
    lcd.print("          ");
    
    lcd.setCursor(0,2);
    lcd.print("SW:");
      if(Array[6] == 1)
      {      
      lcd.setCursor(4,2);
      lcd.print("Good");
      }
      
      else if(Array[6] == 0)
      {
      lcd.setCursor(4,2);
      lcd.print("Low");
      }
    }
   }
 }

  if(mode<1)
    {
  if (radio.available()) {
    
    byte Array[6];
    radio.read(&Array, sizeof(Array));
    
    if(Array[0] == 1)
    {   
      display_NW.showNumberDec(Array[2]);
      
      lcd.setCursor(0,0);
      lcd.print("          ");
      
      lcd.setCursor(0,0);
      lcd.print("L  M  R  |");
      
      lcd.setCursor(0,1);
      lcd.print("          ");
      
      lcd.setCursor(0,1);
      lcd.print(Array[3]);
      
      lcd.setCursor(3,1);
      lcd.print(Array[4]);
      
      lcd.setCursor(6,1);
      lcd.print(Array[5]);
      
    }
    else if(Array[0] == 2)
    {
      display_NE.showNumberDec(Array[2]);
      
      lcd.setCursor(10,0);
      lcd.print("          ");
      
      lcd.setCursor(10,0);
      lcd.print(" L  M  R  ");
      
      lcd.setCursor(11,1);
      lcd.print("          ");
      
      lcd.setCursor(11,1);
      lcd.print(Array[3]);
      
      lcd.setCursor(14,1);
      lcd.print(Array[4]);
      
      lcd.setCursor(17,1);
      lcd.print(Array[5]);
    }
   
      else if(Array[0] == 3)
    {
      display_SW.showNumberDec(Array[2]);
      
      lcd.setCursor(10,2);
      lcd.print("          ");
      
      lcd.setCursor(10,2);
      lcd.print(" L  M  R  ");
      
      lcd.setCursor(11,3);
      lcd.print("          ");
      
      lcd.setCursor(11,3);
      lcd.print(Array[5]);
      
      lcd.setCursor(14,3);
      lcd.print(Array[4]);
      
      lcd.setCursor(17,3);
      lcd.print(Array[3]);
      
    }
    

     else if(Array[0] == 4)
    {
      display_SE.showNumberDec(Array[2]);
      
      lcd.setCursor(0,2);
      lcd.print("          ");
      
      lcd.setCursor(0,2);
      lcd.print("L  M  R  |");
      
      lcd.setCursor(0,3);
      lcd.print("          ");
      
      lcd.setCursor(0,3);
      lcd.print(Array[5]);
      
      lcd.setCursor(3,3);
      lcd.print(Array[4]);
      
      lcd.setCursor(6,3);
      lcd.print(Array[3]);
      
    }
  Serial.print("battery status: ");
  Serial.println(Array[6]);
  
  Serial.print("distance_R: ");
  Serial.println(Array[5]);
  
  Serial.print("distance_M: ");
  Serial.println(Array[4]);
  
  Serial.print("distance_L: ");
  Serial.println(Array[3]);
  
  Serial.print("critical length: ");
  Serial.println(Array[2]);
  
  Serial.print("critical sensor: ");
  Serial.println(Array[1]);
  
  Serial.print("unit: ");
  Serial.println(Array[0]);
  }
 }
}
