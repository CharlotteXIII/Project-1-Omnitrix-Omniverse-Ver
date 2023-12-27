#include <TouchScreen.h> //touch library
#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_KBV.h> //Hardware-specific library
#include <Servo.h>
#include <stdint.h>


#define BLACK        0x0000  /*   0,   0,   0 */
#define GREEN        0x07E0  /*   0, 255,   0 */
#define YP A3  
#define XM A2  
#define YM 9   
#define XP 8
#define MINPRESSURE 5
#define MAXPRESSURE 1000

LCDWIKI_KBV mylcd(240,320,A3,A2,A1,A0,A4);
TouchScreen touch = TouchScreen(XP, YP, XM, YM, 300);
Servo servo;

void color(){
  mylcd.Init_LCD();
  mylcd.Fill_Screen(BLACK);
  // RIGHT
  mylcd.Set_Draw_color(GREEN);
  mylcd.Fill_Triangle(0, 40, 120, 140, 240, 40);
  mylcd.Fill_Rect(0, 0, 240, 40, GREEN);
  mylcd.Set_Draw_color(BLACK);
  mylcd.Fill_Triangle(20, 0, 120,120 , 220, 0);
  // LEFT
  mylcd.Set_Draw_color(GREEN);
  mylcd.Fill_Triangle(0, 280, 120, 180, 240,280);
  mylcd.Fill_Rectangle(0,320,240,280);
  mylcd.Set_Draw_color(BLACK);
  mylcd.Fill_Triangle(20, 320, 120,200 ,220,320);
  
}
void aftertouch(){
  mylcd.Set_Draw_color(GREEN);
  mylcd.Fill_Circle(120 , 160 ,50);
}

void setup() {
  color();
//   servo.attach(3);
//   TSPoint p = touch.getPoint();
//   servo.write(0);
//   if (p.z > MINPRESSURE && p.z < MAXPRESSURE) { 
//     servo.write(15);
//   }
//   else{servo.write(0);}
}


void loop() {
  TSPoint p = touch.getPoint();
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) { 
    aftertouch();
  }
}

