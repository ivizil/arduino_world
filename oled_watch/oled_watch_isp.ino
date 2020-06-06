// Button 1 >>>> 2
// Button 2 >>>> A1
// Button 3 >>>> 3
// Vbat >>>>>>>> A0;

#include "LowPower.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <DS1337.h>
//#define OLED_RESET 4
//Adafruit_SH1106 display(OLED_RESET);
#define OLED_DC     7
#define OLED_CS     6
#define OLED_RESET  8
Adafruit_SH1106 display(OLED_DC, OLED_RESET, OLED_CS);

#include <stdio.h>
DS1337 RTC = DS1337();

int val=0;
const int b1=2;
const int b2=A1;
const int b3=3;
const int vb=A0;
int mode=1;
int modest=0;
int h=0;
int m=0;
int s=0;
int d=0;
int mo=0;
int y=0;
int lock=0;
int volt=0;
float vbat=0;
int timebat=1;
int voltTime=1;

//#define BAT_HEIGHT 16 
//#define BAT_GLCD_WIDTH  7
static const unsigned char PROGMEM bat_bmp0[] =
{ B00111111,B11111111, 
  B00100000,B00000001,
  B11100000,B00000001,
  B11100000,B00000001,
  B11100000,B00000001,
  B00100000,B00000001,
  B00111111,B11111111,};
  
 static const unsigned char PROGMEM bat_bmp1[] =
{ B00111111,B11111111, 
  B00100000,B00000011,
  B11100000,B00000011,
  B11100000,B00000011,
  B11100000,B00000011,
  B00100000,B00000011,
  B00111111,B11111111,};
 
 static const unsigned char PROGMEM bat_bmp2[] =
{ B00111111,B11111111, 
  B00100000,B00000111,
  B11100000,B00000111,
  B11100000,B00000111,
  B11100000,B00000111,
  B00100000,B00000111,
  B00111111,B11111111,};
 
 static const unsigned char PROGMEM bat_bmp3[] =
{ B00111111,B11111111, 
  B00100000,B00001111,
  B11100000,B00001111,
  B11100000,B00001111,
  B11100000,B00001111,
  B00100000,B00001111,
  B00111111,B11111111,};
 
 static const unsigned char PROGMEM bat_bmp4[] =
{ B00111111,B11111111, 
  B00100000,B00011111,
  B11100000,B00011111,
  B11100000,B00011111,
  B11100000,B00011111,
  B00100000,B00011111,
  B00111111,B11111111,};
  
  static const unsigned char PROGMEM bat_bmp5[] =
{ B00111111,B11111111, 
  B00100000,B00111111,
  B11100000,B00111111,
  B11100000,B00111111,
  B11100000,B00111111,
  B00100000,B00111111,
  B00111111,B11111111,};
  static const unsigned char PROGMEM bat_bmp6[] =
{ B00111111,B11111111, 
  B00100000,B01111111,
  B11100000,B01111111,
  B11100000,B01111111,
  B11100000,B01111111,
  B00100000,B01111111,
  B00111111,B11111111,};
 
 static const unsigned char PROGMEM bat_bmp7[] =
{ B00111111,B11111111, 
  B00100000,B11111111,
  B11100000,B11111111,
  B11100000,B11111111,
  B11100000,B11111111,
  B00100000,B11111111,
  B00111111,B11111111,};
 
 static const unsigned char PROGMEM bat_bmp8[] =
{ B00111111,B11111111, 
  B00100001,B11111111,
  B11100001,B11111111,
  B11100001,B11111111,
  B11100001,B11111111,
  B00100001,B11111111,
  B00111111,B11111111,};
 
 static const unsigned char PROGMEM bat_bmp9[] =
{ B00111111,B11111111, 
  B00100011,B11111111,
  B11100011,B11111111,
  B11100011,B11111111,
  B11100011,B11111111,
  B00100011,B11111111,
  B00111111,B11111111,};
 
 static const unsigned char PROGMEM bat_bmp10[] =
{ B00111111,B11111111, 
  B00100111,B11111111,
  B11100111,B11111111,
  B11100111,B11111111,
  B11100111,B11111111,
  B00100111,B11111111,
  B00111111,B11111111,};
 
 static const unsigned char PROGMEM bat_bmp11[] =
{ B00111111,B11111111, 
  B00100111,B11111111,
  B11100111,B11111111,
  B11100111,B11111111,
  B11100111,B11111111,
  B00100111,B11111111,
  B00111111,B11111111,};
 
 static const unsigned char PROGMEM bat_bmp12[] =
{ B00111111,B11111111, 
  B00101111,B11111111,
  B11101111,B11111111,
  B11101111,B11111111,
  B11101111,B11111111,
  B00101111,B11111111,
  B00111111,B11111111,};
 
 static const unsigned char PROGMEM bat_bmp13[] =
{ B00111111,B11111111, 
  B00111111,B11111111,
  B11111111,B11111111,
  B11111111,B11111111,
  B11111111,B11111111,
  B00111111,B11111111,
  B00111111,B11111111,}; 

void wakeUp(){}

void setup() {
 display.begin(SH1106_SWITCHCAPVCC);//, 0x3c
 RTC.start();
 pinMode(b1, INPUT);
 pinMode(b2, INPUT);
 pinMode(vb, INPUT);
 analogReference(INTERNAL);
 //display.setRotation(2); // поворот изображения на 180 градусов
 
}

void loop() { 
time();
battery();
display.display();  
crt_off();
mode=1;
detachInterrupt(0);
display.clearDisplay();
delay(10);
display.SH1106_command(SH1106_DISPLAYON);


for ( val; val<=150;)
{
 RTC.readTime();
 int state1=digitalRead(b1);
 int state2=digitalRead(b2);
 int state3=digitalRead(b3);
 
 
 if (state2==1&&lock==0)
 {
  mode++;
  delay(130);
  state2=0;
 }
 
 if (state3==1)
 {
  display.begin(SH1106_SWITCHCAPVCC);//, 0x3c
  //display.setRotation(2);
  display.clearDisplay(); 
  delay(100);
 }

if (mode==1){
  time();
  battery();
  display.display();
  val++;
  
}


// ++++++++++++++++ MODE SET TIME +++++++++++++++++
while (mode==2)
{
 int state1=digitalRead(b1);
 int state2=digitalRead(b2);
 int state3=digitalRead(b3);
  
  lock=1;
  if (state2==1)
  { 
    modest++;
    delay(150);
    state2=0;
  }
  //+++++++++++++++++++ Set HOURS++++++++++++++++++++
  if (modest==1)
  {
display.clearDisplay();
 display.setTextSize(3);
display.setTextColor(BLACK,WHITE);
 display.setCursor(0,20);
 if (h<=9)
 {display.print("0");}
 display.print(h);
 display.setTextColor(WHITE);
 display.print(":");
 if (m<=9)
 {display.print("0");}
 display.print(m);
 display.setTextSize(2);
 display.setCursor(90,28);
 display.print(":");
 if (s<=9)
 {display.print("0"); }
 display.print(s);
 battery();
 display.setTextSize(1);
 display.setCursor(40,50);
 display.print(d);
 display.print('/');
 display.print(mo);
 display.print('/');
 display.print(y);
 display.display();
 if (state1==1)
 {
   h++;
   delay(150);
 }
 if (state3==1)
 {
   h--;
   delay(150);
 }
 
 if (h>=24)
 { h=0;}
 
 if (h<0)
 {h=23;}
 
 if (state2==1)
 {
   modest++;
   delay(150);
   state2=0;
 }
 
  }
// +++++++++++++++++++++++++++++ SET MIN ++++++++++++++++++  
  if (modest==2)
  {
display.clearDisplay();
 display.setTextSize(3);
 display.setTextColor(WHITE);
 display.setCursor(0,20);
 if (h<=9)
 {display.print("0");}
 display.print(h);
 display.print(":");
 display.setTextColor(BLACK,WHITE);
 if (m<=9)
 {display.print("0");}
 
 display.print(m);
 display.setTextColor(WHITE);
 display.setTextSize(2);
 display.setCursor(90,28);
 display.print(":");
 if (s<=9)
 {display.print("0"); }
 display.print(s);
 battery();
 display.setTextSize(1);
 display.setCursor(40,50);
 display.print(d);
 display.print('/');
 display.print(mo);
 display.print('/');
 display.print(y);
 display.display();
 if (state1==1)
 {
   m++;
   delay(100);
 }
 if (state3==1)
 {
   m--;
   delay(150);
 }
 
 if (m>=60)
 { m=0;}

 if (m<0)
 { m=59;}
 
 if (state2==1)
 {
   modest++;
   delay(150);
   state2=0;
 }
 }
 
 // +++++++++++++++++++++++ SET SEC +++++++++++++++++++++
 if (modest==3)
  {
 display.clearDisplay();
 display.setTextSize(3);
 display.setTextColor(WHITE);
 display.setCursor(0,20);
 if (h<=9)
 {display.print("0");}
 display.print(h);
 display.print(":");
 if (m<=9)
 {display.print("0");}
 display.print(m);
 display.setTextSize(2);
 display.setCursor(90,28);
 display.print(":");
 display.setTextColor(BLACK,WHITE);
 if (s<=9)
 {display.print("0"); }
 display.print(s);
 display.setTextColor(WHITE);
 battery();
 display.setTextSize(1);
 display.setCursor(40,50);
 display.print(d);
 display.print('/');
 display.print(mo);
 display.print('/');
 display.print(y);
 display.display();
 if (state1==1)
 {
   s++;
   delay(100);
 }
 if (state3==1)
 {
   s--;
   delay(150);
 }
 
 if (s>=60)
 { s=0;}

 if (s<0)
 { s=59;}
 
 if (state2==1)
 {
   modest++;
   delay(150);
   state2=0;
 }
 }
  // +++++++++++++++++++++++  END SET SEC +++++++++++++++++++++
  
  // +++++++++++++++++++++++  SET DAY +++++++++++++++++++++
 if (modest==4)
  {
 display.clearDisplay();
 display.setTextSize(3);
 display.setTextColor(WHITE);
 display.setCursor(0,20);
 if (h<=9)
 {display.print("0");}
 display.print(h);
 display.print(":");
 if (m<=9)
 {display.print("0");}
 display.print(m);
 display.setTextSize(2);
 display.setCursor(90,28);
 display.print(":");
 if (s<=9)
 {display.print("0"); }
 display.print(s);
 display.setTextColor(WHITE);
 battery();
 display.setTextSize(1);
 display.setCursor(40,50);
 display.setTextColor(BLACK,WHITE);
 display.print(d);
 display.setTextColor(WHITE);
 display.print('/');
 display.print(mo);
 display.print('/');
 display.print(y); 
 display.display();
 if (state1==1)
 {
   d++;
   delay(100);
 }
 if (state3==1)
 {
   d--;
   delay(150);
 }
 
 if (d>=32)
 { d=1;}

 if (d<0)
 { d=31;}
 
 if (state2==1)
 {
   modest++;
   delay(150);
   state2=0;
 }
 } 
 // +++++++++++++++++++++++  END SET DAY +++++++++++++++++++++ 

  // +++++++++++++++++++++++  SET MON +++++++++++++++++++++
 if (modest==5)
  {
 display.clearDisplay();
 display.setTextSize(3);
 display.setTextColor(WHITE);
 display.setCursor(0,20);
 if (h<=9)
 {display.print("0");}
 display.print(h);
 display.print(":");
 if (m<=9)
 {display.print("0");}
 display.print(m);
 display.setTextSize(2);
 display.setCursor(90,28);
 display.print(":");
 if (s<=9)
 {display.print("0"); }
 display.print(s);
 display.setTextColor(WHITE);
 battery();
 display.setTextSize(1);
 display.setCursor(40,50);
 display.print(d);
 display.print('/');
 display.setTextColor(BLACK,WHITE);
 display.print(mo);
 display.setTextColor(WHITE);
 display.print('/');
 display.print(y); 
 display.display();
 if (state1==1)
 {
   mo++;
   delay(100);
 }
 if (state3==1)
 {
   mo--;
   delay(150);
 }
 
 if (mo>=13)
 { mo=1;}

 if (mo<=0)
 { mo=12;}
 
 if (state2==1)
 {
   modest++;
   delay(150);
   state2=0;
 }
 } 
 // +++++++++++++++++++++++  END SET DAY +++++++++++++++++++++  
 
  // +++++++++++++++++++++++  SET YEAR +++++++++++++++++++++
 if (modest==6)
  {
 display.clearDisplay();
 display.setTextSize(3);
 display.setTextColor(WHITE);
 display.setCursor(0,20);
 if (h<=9)
 {display.print("0");}
 display.print(h);
 display.print(":");
 if (m<=9)
 {display.print("0");}
 display.print(m);
 display.setTextSize(2);
 display.setCursor(90,28);
 display.print(":");
 if (s<=9)
 {display.print("0"); }
 display.print(s);
 display.setTextColor(WHITE);
 battery();
 display.setTextSize(1);
 display.setCursor(40,50);
 display.print(d);
 display.print('/');
 display.print(mo);
 display.print('/');
 display.setTextColor(BLACK,WHITE);
 display.print(y); 
 display.display();
 if (state1==1)
 {
   y++;
   delay(100);
 }
 if (state3==1)
 {
   y--;
   delay(150);
 }
 
 if (y>=2020)
 { y=2000;}
 
 if (state2==1)
 {
   modest++;
   delay(150);
   state2=0;
 }
 } 
 // +++++++++++++++++++++++  END SET YEAR +++++++++++++++++++++  
 
// ++++++++++++++++++++++ WRITE to DS1337 +++++++++++++++

 if (modest==7) 
 {
    RTC.setSeconds(s);
    RTC.setMinutes(m);
    RTC.setHours(h);
    RTC.setDays(d);
    RTC.setMonths(mo);
    RTC.setYears(y);
    RTC.writeTime();
  delay(1);
  lock=0;
  modest=1;
  mode=1;
  val=0;
 }    
}
val++;
 
}
val=0;
display.clearDisplay();
//delay(50);
}

void battery(void)
{
 if (voltTime==1)
 {
 volt=analogRead(vb);
 vbat=volt*0.0045;
 voltTime=5;
 }
 voltTime--;
 
 if (2>vbat && vbat>=1) {display.drawBitmap(0,0,bat_bmp0, 16, 7, 1);}
 if (2.1>vbat && vbat>=2) {display.drawBitmap(0,0,bat_bmp1, 16, 7, 1);}
 if (2.2>vbat && vbat>=2.1) {display.drawBitmap(0,0,bat_bmp2, 16, 7, 1);}
 if (2.3>vbat && vbat>=2.2) {display.drawBitmap(0,0,bat_bmp3, 16, 7, 1);}
 if (2.4>vbat && vbat>=2.3) {display.drawBitmap(0,0,bat_bmp4, 16, 7, 1);}
 if (2.5>vbat && vbat>=2.4) {display.drawBitmap(0,0,bat_bmp5, 16, 7, 1);}
 if (2.6>vbat && vbat>=2.5) {display.drawBitmap(0,0,bat_bmp6, 16, 7, 1);}
 if (2.7>vbat && vbat>=2.6) {display.drawBitmap(0,0,bat_bmp7, 16, 7, 1);}
 if (2.8>vbat && vbat>=2.7) {display.drawBitmap(0,0,bat_bmp8, 16, 7, 1);}
 if (2.9>vbat && vbat>=2.8) {display.drawBitmap(0,0,bat_bmp9, 16, 7, 1);}
 if (3>vbat && vbat>=2.9) {display.drawBitmap(0,0,bat_bmp10, 16, 7, 1);}
 if (3.1>vbat && vbat>=3) {display.drawBitmap(0,0,bat_bmp11, 16, 7, 1);}
 if (3.2>vbat && vbat>=3.1) {display.drawBitmap(0,0,bat_bmp12, 16, 7, 1);}
 if (4>vbat && vbat>=3.2) {display.drawBitmap(0,0,bat_bmp13, 16, 7, 1);}
 
 /*
 if (2>vbat && vbat>=1) {display.drawRect(0,2,2,4,1);display.drawRect(2,0,14,8,1);}
 if (2.1>vbat && vbat>=2) {display.drawRect(0,2,2,4,1);display.drawRect(2,0,14,8,1);display.fillRect(13,1,13,1,1);}
 if (2.2>vbat && vbat>=2.1) {display.drawRect(0,2,2,4,1);display.drawRect(2,0,14,8,1);display.fillRect(13,1,2,6,1);}
 if (2.3>vbat && vbat>=2.2) {display.drawRect(0,2,2,4,1);display.drawRect(2,0,14,8,1);display.fillRect(12,1,3,6,1);}
 if (2.4>vbat && vbat>=2.3) {display.drawRect(0,2,2,4,1);display.drawRect(2,0,14,8,1);display.fillRect(11,1,4,6,1);}
 if (2.5>vbat && vbat>=2.4) {display.drawRect(0,2,2,4,1);display.drawRect(2,0,14,8,1);display.fillRect(10,1,5,6,1);}
 if (2.6>vbat && vbat>=2.5) {display.drawRect(0,2,2,4,1);display.drawRect(2,0,14,8,1);display.fillRect(9,1,6,6,1);}
 if (2.7>vbat && vbat>=2.6) {display.drawRect(0,2,2,4,1);display.drawRect(2,0,14,8,1);display.fillRect(8,1,7,6,1);}
 if (2.8>vbat && vbat>=2.7) {display.drawRect(0,2,2,4,1);display.drawRect(2,0,14,8,1);display.fillRect(7,1,8,6,1);}
 if (2.9>vbat && vbat>=2.8) {display.drawRect(0,2,2,4,1);display.drawRect(2,0,14,8,1);display.fillRect(6,1,9,6,1);}
 if (3>vbat && vbat>=2.9) {display.drawRect(0,2,2,4,1);display.drawRect(2,0,14,8,1);display.fillRect(5,1,10,6,1);}
 if (3.1>vbat && vbat>=3) {display.drawRect(0,2,2,4,1);display.drawRect(2,0,14,8,1);display.fillRect(4,1,11,6,1); }
 if (3.2>vbat && vbat>=3.1){display.drawRect(0,2,2,4,1);display.drawRect(2,0,14,8,1);display.fillRect(3,1,12,6,1);}
 if (4>vbat && vbat>=3.2) {display.drawRect(0,2,2,4,1);display.drawRect(2,0,14,8,1);display.fillRect(2,1,13,6,1);}
 */
 display.setTextSize(1);
 display.setCursor(90,0);
 display.print(vbat);
 display.print(" V"); 
}

void time(void)
{
  h=RTC.getHours();
  m=RTC.getMinutes();
  s=RTC.getSeconds();
  d=RTC.getDays();
  mo=RTC.getMonths();
  y=RTC.getYears();
 
 display.clearDisplay();
 display.setTextSize(3);
 display.setTextColor(WHITE);
 display.setCursor(0,20);
 if (h<=9)
 {display.print("0");}
 display.print(h);
 display.print(":");
 if (m<=9)
 {display.print("0");}
 display.print(m);
 display.setTextSize(2);
 display.setCursor(90,28);
 display.print(":");
 if (s<=9)
 {display.print("0"); }
 display.print(s);
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.setCursor(40,50);
 display.print(d);
 display.print('/');
 display.print(mo);
 display.print('/');
 display.print(y);
}

void show_time(void)
{
 display.clearDisplay();
 display.setTextSize(3);
 display.setTextColor(WHITE);
 display.setCursor(0,20);
 if (h<=9)
 {display.print("0");}
 display.print(h);
 display.print(":");
 if (m<=9)
 {display.print("0");}
 display.print(m);
 display.setTextSize(2);
 display.setCursor(90,28);
 display.print(":");
 if (s<=9)
 {display.print("0"); }
 display.print(s);
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.setCursor(40,50);
 display.print(d);
 display.print('/');
 display.print(mo);
 display.print('/');
 display.print(y); 
}

void crt_off(void)
{
  for (int16_t i=0; i<33; i++)
  { 
  display.drawLine(0, i, 127, i, WHITE);
  display.drawLine(0, i-1, 127, i-1, BLACK);
  display.drawLine(0, 64-i, 127, 64-i, WHITE);
  display.drawLine(0, 65-i, 127, 65-i, BLACK);
  display.display();
  }
  
  for (int16_t i=0; i<65; i++)
  { 
  display.drawLine(i-1, 32, 128-i, 32, BLACK);  
  display.drawLine(i, 32, 127-i, 32, WHITE);
  display.display();
  }
  display.SH1106_command(SH1106_DISPLAYOFF);
  attachInterrupt(0,wakeUp, HIGH);
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  
}
  
