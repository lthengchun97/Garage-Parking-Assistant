#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <SPI.h>

#include <stdio.h>
#include <iostream>
#include <Adafruit_ILI9341esp.h>
#include <Adafruit_GFX.h>
#include <XPT2046.h>
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>
#include "font12.h"

// Modify the following two lines to match your hardware
// Also, update calibration parameters below, as necessary

// For the esp shield, these are the default.
#define TFT_DC 2
#define TFT_CS 15
#define TOUCH_IRQ 5
#define TOUCH_CS  4
#define ULT_ECHO  0
#define ULT_TRIG  16

// For the page configuration
#define MAIN_PAGE 1
#define START_PAGE 2
#define OPTION_PAGE 3
#define SENSOR_PAGE 4
#define SET_GREEN_PAGE  5
#define SET_RED_PAGE    6
#define SET_YELLOW_PAGE 7

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
//Adafruit_ILI9341 tft1 = Adafruit_ILI9341(TFT_CS, TFT_DC);
XPT2046 touch(TOUCH_CS,TOUCH_IRQ);

Adafruit_GFX_Button button,button1;

int currentPage;
int greenTh,redTh,yellowTh;   // Create threshold values
int tempGreenTh,tempRedTh,tempYellowTh;   // Store the temporary threshold values
int answer =0;
char lastchar = ' ';
char keyarray[4];
int32_t keyarrayi=0;


char calButton[4][3] = {
  { '7', '8', '9' },
  { '4', '5', '6' },
  { '1', '2', '3' },
  { 'C', '0', '=' }
};

void setup() {
  delay(1000);

  pinMode(ULT_TRIG, OUTPUT);
  pinMode(ULT_ECHO, INPUT);
  Serial.begin(115200);
  SPI.setFrequency(ESP_SPI_FREQ);

  tft.begin();
  touch.begin(tft.width(), tft.height());  // Must be done before setting rotation
  Serial.println("");
  Serial.print("tftx ="); Serial.println(tft.width()); Serial.print("tfty ="); Serial.println(tft.height());
  tft.fillScreen(ILI9341_BLACK);
  // Replace these for your screen module
  touch.setCalibration(209, 1759, 1775, 273);
  tft.setCursor(15,130);
  tft.setTextSize(3);
  tft.setFont();
  tft.setTextColor(ILI9341_YELLOW);
  tft.print("DESIGNED BY:");

  tft.setCursor(15,160);
  tft.setTextSize(2);
  tft.print("LEE THENG CHUN");

  delay(1000);

  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(15,140);
  tft.setTextSize(2);
  tft.setFont();
  tft.setTextColor(ILI9341_YELLOW);
  tft.print("GARAGE PARKING");
  tft.setCursor(15,160);
  tft.print("ASSISTANT");

  currentPage = MAIN_PAGE;
  
  greenTh=30;
  yellowTh=20;
  redTh=10;
  
  delay(1000);
  //button.initButton(&tft, 100, 100, 70, 40, ILI9341_DARKCYAN, ILI9341_BLUE, ILI9341_GREENYELLOW, "Clear", 2);
  //button1.initButton(&tft, 50, 50, 40, 30, ILI9341_DARKCYAN, ILI9341_BLUE, ILI9341_GREEN, "What", 2);
  //button.drawButton();
  //button1.drawButton();

  mainPage();
}

static uint16_t prev_x = 0xffff, prev_y = 0xffff;

void mainPage(){
  //currentPage=MAIN_PAGE;
  tft.fillScreen(ILI9341_BLACK);
  tft.drawRoundRect(10,10,220,310,8,ILI9341_WHITE);
  
  tft.fillRoundRect(40,230,160,50,8,ILI9341_PURPLE);    // Touch Screen Sensor Test
  tft.drawRoundRect(40,230,160,50,8,ILI9341_YELLOW);

  tft.fillRoundRect(40,150,160,50,8,ILI9341_PURPLE);    // Configuration
  tft.drawRoundRect(40,150,160,50,8,ILI9341_YELLOW);

  tft.fillRoundRect(40,70,160,50,8,ILI9341_PURPLE);     // Start
  tft.drawRoundRect(40,70,160,50,8,ILI9341_YELLOW);

  tft.setCursor(47,25);
  tft.setTextSize(1);
  tft.setFont(&FreeMonoBold9pt7b);
  tft.setTextColor(ILI9341_RED);
  tft.print("GARAGE PARKING");
  tft.setCursor (70,55);
  tft.print("ASSISTANT");

  tft.setTextSize(2);
  tft.setTextColor(ILI9341_NAVY);
  tft.setCursor(65,103);
  tft.print("START");

  tft.setCursor(55,183);
  tft.print("OPTION");

  tft.setFont(&FreeMonoBold12pt7b);
  tft.setTextSize(1);
  tft.setCursor(45,263);
  tft.print("SENSOR TEST");
  
  }

void startOperation(){
  tft.fillScreen(ILI9341_BLACK);
  tft.fillRoundRect(2,2,58,48,8,ILI9341_WHITE);    // Back Symbol
  tft.drawRoundRect(2,2,58,48,8,ILI9341_BLACK);
  tft.setCursor(25,30);
  tft.setTextSize(1);
  tft.setFont(&FreeMonoBold9pt7b);
  tft.setTextColor(ILI9341_BLACK);
  tft.print("<");
  }
  
void optionOperation(){
  tft.fillScreen(ILI9341_BLACK);
  tft.fillRoundRect(2,2,58,38,8,ILI9341_WHITE);    // Back Symbol
  tft.drawRoundRect(2,2,58,38,8,ILI9341_BLACK);

  tft.drawRoundRect(145,75,75,32,8,ILI9341_GREEN);    // Green Threshold Set
  
  tft.drawRoundRect(145,155,75,32,8,ILI9341_YELLOW);    // Yellow Threshold Set
  
  tft.drawRoundRect(145,235,75,32,8,ILI9341_RED);    // Red Threshold Set

  tft.drawRoundRect(70,285,95,25,8,ILI9341_WHITE);   // Finish Set
  
  tft.setCursor(25,25);
  tft.setTextSize(1);
  tft.setFont(&FreeMonoBold9pt7b);
  tft.setTextColor(ILI9341_BLACK);
  tft.print("<");

  tft.setCursor (45,60);
  tft.setTextColor(ILI9341_GREEN);
  tft.print("GREEN THRESHOLD");
  tft.setCursor (20,100);
  tft.setTextSize(2);
  tft.print(greenTh);
  tft.setCursor(80,100);
  tft.print("cm");
  tft.setCursor(150,100);
  tft.print("SET");

  tft.setTextSize(1);
  tft.setCursor(38,140);
  tft.setTextColor(ILI9341_YELLOW);
  tft.print("YELLOW THRESHOLD");
  tft.setCursor (20,180);
  tft.setTextSize(2);
  tft.print(yellowTh);
  tft.setCursor(80,180);
  tft.print("cm");
  tft.setCursor(150,180);
  tft.print("SET");

  tft.setTextSize(1);
  tft.setCursor(52,220);
  tft.setTextColor(ILI9341_RED);
  tft.print("RED THRESHOLD");
  tft.setCursor (20,260);
  tft.setTextSize(2);
  tft.print(redTh);
  tft.setCursor(80,260);
  tft.print("cm");
  tft.setCursor(150,260);
  tft.print("SET");

  tft.setTextSize(1);
  tft.setCursor(75,300);
  tft.setTextColor(ILI9341_WHITE);
  tft.print("FINISHED");
  
  }

void sensorOperation(){
  tft.fillScreen(ILI9341_BLUE);
  tft.fillRoundRect(2,2,58,38,8,ILI9341_WHITE);    // Back Symbol
  tft.drawRoundRect(2,2,58,38,8,ILI9341_BLACK);
  tft.setCursor(25,25);
  tft.setTextSize(1);
  tft.setFont(&FreeMonoBold9pt7b);
  tft.setTextColor(ILI9341_BLACK);
  tft.print("<");
  }



void drawNumPad()
{
  tft.setFont(&FreeMonoBold9pt7b);
  tft.setTextSize(2);
  tft.fillScreen(ILI9341_BLACK);
  
  tft.drawRoundRect(10,10,220,60,8,ILI9341_ORANGE);

  for(int i=0;i<3;i++){
    for (int j=0;j<3;j++){
      tft.drawRoundRect((20+i*(70)),(80+j*(60)),60,50,8,ILI9341_BLUE);
      tft.setCursor((20+i*(70))+20,(80+j*(60))+33);
      tft.setTextColor(ILI9341_WHITE);
      tft.print(calButton[j][i]);
      }
  }
  tft.drawRoundRect(20,260,60,50,8,ILI9341_BLUE);
  tft.setCursor(40,293);
  tft.setTextColor(ILI9341_WHITE);
  tft.print(calButton[3][1]);
  
  tft.drawRoundRect(90,260,140,50,8,ILI9341_GREEN);
  tft.setCursor(110,295);
  tft.setTextColor(ILI9341_GREEN);
  tft.print("ENTER");

  //tft.setCursor(140,50);
  //tft.setTextColor(ILI9341_GREEN);
  //tft.print(answer);
  
}

void addchar(char *array1, char inchar)
{
  int32_t input_str_len = strlen(array1);  //get current length of string
  array1[input_str_len]   = inchar;  //assign received character to the last position in the string
  array1[input_str_len + 1] = '\0';
}

int32_t chartoint(char num)
{

  return num - '0';
}

int32_t chararraytoint(char *a)
{
  int32_t len = strlen(a);
  int32_t finalnum = 0;
  int32_t intval = 0;
  int32_t placeval = 1;

  if (len > 0)
  {
    for (int i = len - 1; i >= 0; i--)
    {
      intval = chartoint(a[i]);
      intval = intval * placeval;
      finalnum = finalnum + intval;
      placeval *= 10;
    }
  }
  return finalnum;
}

void loop() {
  //int stoploop;
  uint16_t x, y;

  if(currentPage == MAIN_PAGE){
  if (touch.isTouching()) {
    touch.getPosition(x, y);
    Serial.print("x ="); Serial.print(x); Serial.print(" y ="); Serial.println(y);
    if ((x>40 && x<200) && (y>70 && y<110)) {
      Serial.println("Start Page");
      currentPage=START_PAGE;
      startOperation();
    } 
    else if ((x>40 && x<200) && (y>150 && y<200))
    {
      Serial.println("Option Page");
      currentPage= OPTION_PAGE;
      optionOperation();
    }
    else if ((x>40 && x<200) && (y>230 && y<280))
    {
      Serial.println("Sensor Page");
      currentPage = SENSOR_PAGE;
      sensorOperation();
    }
  } 
  }
  
  if(currentPage == START_PAGE){
    long duration, distance;
    uint16_t x, y;
    digitalWrite(ULT_TRIG, LOW);  
    delayMicroseconds(2); 
  
    digitalWrite(ULT_TRIG, HIGH);
    delayMicroseconds(10); 
  
    digitalWrite(ULT_TRIG, LOW);
    duration = pulseIn(ULT_ECHO, HIGH);
    distance = (duration/2) / 29.1;

    Serial.print("Centimeter:");
    Serial.println(distance);
  
    delay(200);

    if (distance > greenTh && greenTh > redTh && greenTh > yellowTh){
      tft.fillScreen(ILI9341_GREEN);
      tft.setCursor(80,110);
      tft.setTextSize(4);
      tft.setFont();
      tft.setTextColor(ILI9341_BLACK);
      tft.print("SAFE");
      tft.setTextSize(6);
      if(distance < 1000){
        tft.setCursor(90,160);
        tft.print(distance);
        }
      else{
        tft.setCursor(65,160);
        tft.print(distance);
        }
    }
    else if ((distance >= yellowTh || distance <=yellowTh) && distance <= greenTh && distance >= redTh){
      tft.fillScreen(ILI9341_YELLOW);
      tft.setCursor(15,110);
      tft.setTextSize(4);
      tft.setFont();
      tft.setTextColor(ILI9341_BLACK);
      tft.print("SLOW DOWN");
      tft.setTextSize(6);
      tft.setCursor(90,160);
      tft.print(distance);
      }
    else if (distance < redTh && distance < greenTh && distance < yellowTh){
      tft.fillScreen(ILI9341_RED);
      tft.setCursor(80,110);
      tft.setTextSize(4);
      tft.setFont();
      tft.setTextColor(ILI9341_BLACK);
      tft.print("STOP");
      tft.setTextSize(6);
      tft.setCursor(100,160);
      tft.print(distance);
      }
    else{
      tft.fillScreen(ILI9341_BLACK);
      tft.setCursor(60,130);
      tft.setTextSize(4);
      tft.setFont();
      tft.setTextColor(ILI9341_RED);
      tft.print("ERROR");
      }
 
    delay(20);
    
    if (touch.isTouching()) {
      touch.getPosition(x, y);
      Serial.print("x ="); Serial.print(x); Serial.print(" y ="); Serial.println(y);
      currentPage = MAIN_PAGE;
      mainPage();
      return;
    }
    }

  if (currentPage == OPTION_PAGE){
    uint16_t x, y;
    if (touch.isTouching()) {
      touch.getPosition(x, y);
      Serial.print("x ="); Serial.print(x); Serial.print(" y ="); Serial.println(y);
      if ((x>2 && x<60) && (y>2 && y<40)) {
        currentPage = MAIN_PAGE;
        mainPage();
        return;
      }
      if ((x>145&&x<220)&&(y>75&&y<107)){
        answer = greenTh;
        drawNumPad();
        currentPage = SET_GREEN_PAGE;
        tft.setCursor(100,50);
        tft.setTextColor(ILI9341_GREEN);
        }
      if ((x>145&&x<220)&&(y>155&&y<187)){
        answer = yellowTh;
        drawNumPad();
        currentPage = SET_YELLOW_PAGE;
        tft.setCursor(100,50);
        tft.setTextColor(ILI9341_GREEN);
        }
      if ((x>145&&x<220)&&(y>235&&y<267)){
        answer = redTh;
        drawNumPad();
        currentPage = SET_RED_PAGE;
        tft.setCursor(100,50);
        tft.setTextColor(ILI9341_GREEN);
        }
      if ((x>70&&x<165)&&(y>285&&y<310)){
        // Further development : do a confirmation/ checking before return
        currentPage = MAIN_PAGE;
        mainPage();
        return; 
        }
      
    }
  }
  if (currentPage == SENSOR_PAGE){
    uint16_t x, y;
    if (touch.isTouching()) {
      touch.getPosition(x, y);
      Serial.print("x ="); Serial.print(x); Serial.print(" y ="); Serial.println(y);
      if ((x>2 && x<60) && (y>2 && y<40)) {
        currentPage = MAIN_PAGE;
        mainPage();
        return;
      }
    } 
    }
    
  if (currentPage == SET_GREEN_PAGE || currentPage == SET_YELLOW_PAGE || currentPage == SET_RED_PAGE){
    
    if (touch.isTouching()) {
      delay(200);
      touch.getPosition(x, y);
      Serial.print("x ="); Serial.print(x); Serial.print(" y ="); Serial.println(y);
      if ((x>90 && x<230) && (y>260 && y<310)) {
        keyarray[0] = '\0';
        switch(currentPage){
          case SET_GREEN_PAGE :   greenTh = keyarrayi;
                                  keyarrayi=0;
                                  break;
          case SET_YELLOW_PAGE:   yellowTh = keyarrayi;
                                  keyarrayi=0;
                                  break;
          case SET_RED_PAGE:      redTh = keyarrayi;
                                  keyarrayi=0;
                                  break;
          default: greenTh = greenTh;
                   yellowTh = yellowTh;
                   redTh = redTh;
                   keyarrayi=0;
          }
        currentPage = OPTION_PAGE;
        optionOperation();
        return;
      }
      if((x>20&&x<80)&&(y>80&&y<130)){
        addchar(keyarray,'7');
        keyarrayi=chararraytoint(keyarray);
        tft.print(7);
        }
      if((x>90&&x<150)&&(y>80&&y<130)){
        addchar(keyarray,'8');
        keyarrayi=chararraytoint(keyarray);
        tft.print(8);
        }
      if((x>160&&x<210)&&(y>80&&y<130)){
        addchar(keyarray,'9');
        keyarrayi=chararraytoint(keyarray);
        tft.print(9);
        }
      if((x>20&&x<80)&&(y>140&&y<190)){
        addchar(keyarray,'4');
        keyarrayi=chararraytoint(keyarray);
        tft.print(4);
        }
      if((x>90&&x<150)&&(y>140&&y<190)){
        addchar(keyarray,'5');
        keyarrayi=chararraytoint(keyarray);
        tft.print(5);
        }
      if((x>160&&x<210)&&(y>140&&y<190)){
        addchar(keyarray,'6');
        keyarrayi=chararraytoint(keyarray);
        tft.print(6);
        }
      if((x>20&&x<80)&&(y>200&&y<250)){
        addchar(keyarray,'1');
        keyarrayi=chararraytoint(keyarray);
        tft.print(1);
        }
      if((x>90&&x<150)&&(y>200&&y<250)){
        addchar(keyarray,'2');
        keyarrayi=chararraytoint(keyarray);
        tft.print(2);
        }
      if((x>160&&x<210)&&(y>200&&y<250)){
        addchar(keyarray,'3');
        keyarrayi=chararraytoint(keyarray);
        tft.print(3);
        }
      if((x>20&&x<80)&&(y>260&&y<310)){
        addchar(keyarray,'0');
        keyarrayi=chararraytoint(keyarray);
        tft.print(0);
        }
      }
  }
    }
