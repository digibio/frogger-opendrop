/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "OpenDropDisplay.h"

// If using software SPI (the default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#define POL_pin         3           // POL Pin
#define LE_pin          4           // LE Pin
#define SCK_pin         15           // SCK Pin
#define DI1_pin         16           // Digital Out Pin 1
#define DI2_pin         2           // Digital Out Pin 2

#define BUT1_pin         8           // Buton 1 pin
#define BUT2_pin         A5           // Buton 1 pin
#define BUT3_pin         7           // Buton 1 pin
#define BUT4_pin         A4           // Buton 1 pin
#define LEFT         8           // Buton 1 pin
#define RIGHT         A5           // Buton 1 pin
#define DOWN         7           // Buton 1 pin
#define UP         A4           // Buton 1 pin

#define light_pin         5           // Digital  Pin for LEDs
#define speaker_pin       6           // Digital Pin for speaker

#define WIDTH         16
#define HEIGHT         8



boolean sound_flag=true;           // Digital Out Pin 2
boolean light_flag=false;           // Digital Out Pin 2

int frog_x = 8;
int frog_y = 0;
uint32_t lastTick;
int road_timer;
#define CAR_SPEED  200

byte car_state=0;


// the loop function runs over and over again forever
void loop() {
  long time = millis();
  long dt = time - lastTick;
  lastTick = time;
    
  if( !digitalRead(LEFT) && frog_x > 0 ) {
    frogmove(frog_x-1,frog_y);
  }
  if( !digitalRead(RIGHT) && frog_x < WIDTH-1 ) {
    frogmove(frog_x+1,frog_y);
  }
  if( !digitalRead(UP) && frog_y > 0 ) {
    frogmove(frog_x,frog_y-1);
  }
  if( !digitalRead(DOWN) && frog_y < HEIGHT-1 ) {
    frogmove(frog_x,frog_y+1);
  }

  road_timer += dt;
  if (road_timer > CAR_SPEED) {
    road_timer = road_timer-CAR_SPEED;
    move_cars();
    update_hvchip();
    display.display();
  }
}

void frogmove(int nx,int ny){
  setPad(nx,ny,1);
  setPad(frog_x,frog_y,0);
  frog_x=nx;
  frog_y=ny;
  frogger();
}

void setCarFluxelState(byte carpos, boolean state)
{
  // translate carpos into x and y
  byte x,y;
  carpos=carpos%36;
  
  const byte TOPROAD=2;
  const byte BOTTOMROAD=5;
  const byte LEFTROAD=0;
  const byte RIGHTROAD=WIDTH-1;

  const byte BR=18;
  const byte BL=33;
  const byte LEN=36;
  const byte TR=15;
  
  if(carpos <= TR) { 
    x = carpos; y = TOPROAD;
  }
  else if(carpos <= BR) {
    x = RIGHTROAD; y = (carpos-TR) + TOPROAD;
  }
  else if(carpos <= BL) {
    x = BL - carpos + LEFTROAD; y=BOTTOMROAD;
  }
  else {
    x = LEFTROAD; y = LEN-carpos + TOPROAD;
  }
  
  setPad(x,y,state);
}

void move_cars() {
  const byte DIST=12;
  const byte CARS=36/DIST;
  
  for (byte i=0;i<CARS;i++) {
    byte p=i*DIST+car_state;
    setCarFluxelState((p+1)%36, 1);
    setCarFluxelState(p%36, 0);
  }
  car_state=(car_state+1)%36;
}

void bit_sound()
{

  long t = 0; 
int v = 0; 
unsigned int c3 = 2000;
unsigned int c4 = 6;

for (int i=0; i < 3800; i++){
  

  v = t * ((t>>15|t>>c4)&83&t>>(c4>>3));
     analogWrite (speaker_pin, v);
 
     delayMicroseconds(c3);
     t++;
}
     analogWrite (speaker_pin, 0);


  }

void frogger()
{

const float vol1=0.5;
const float vol2=0.3;
const float vol3=0.1;
const float vol4=0.05;
const float vol5=0.02;
const float vol6=0.01;

const int pitch1=1700; //586
const int pitch2=1664; //733
const int pitch3=1130; //885
const int pitch4=858;  //1166
const int pitch5=1015;  //986

//586
for (int i=0; i < 5; i++){
   digitalWrite(speaker_pin, HIGH);   // sets the pin on
   delayMicroseconds(pitch1*vol1);        // pauses for 50 microseconds      
   digitalWrite(speaker_pin, LOW);    // sets the pin off
   delayMicroseconds(pitch1*(1-vol1));        // pauses for 50 microseconds      
}


//733
for (int i=0; i < 5; i++){
   digitalWrite(speaker_pin, HIGH);   // sets the pin on
   delayMicroseconds(pitch2*vol1);        // pauses for 50 microseconds      
   digitalWrite(speaker_pin, LOW);    // sets the pin off
   delayMicroseconds(pitch2*(1-vol1));        // pauses for 50 microseconds      
}


//885

for (int i=0; i < 5; i++){
   digitalWrite(speaker_pin, HIGH);   // sets the pin on
   delayMicroseconds(pitch3*vol1);        // pauses for 50 microseconds      
   digitalWrite(speaker_pin, LOW);    // sets the pin off
   delayMicroseconds(pitch2*(1-vol1));        // pauses for 50 microseconds      
}



//1166

for (int i=0; i < 5; i++){
   digitalWrite(speaker_pin, HIGH);   // sets the pin on
   delayMicroseconds(pitch4*vol1);        // pauses for 50 microseconds      
   digitalWrite(speaker_pin, LOW);    // sets the pin off
   delayMicroseconds(pitch2*(1-vol1));        // pauses for 50 microseconds      
}


//986

for (int i=0; i < 5; i++){
   digitalWrite(speaker_pin, HIGH);   // sets the pin on
   delayMicroseconds(pitch5*vol1);        // pauses for 50 microseconds      
   digitalWrite(speaker_pin, LOW);    // sets the pin off
   delayMicroseconds(pitch2*(1-vol1));        // pauses for 50 microseconds      
}

for (int i=0; i < 10; i++){
   digitalWrite(speaker_pin, HIGH);   // sets the pin on
   delayMicroseconds(pitch5*vol2);        // pauses for 50 microseconds      
   digitalWrite(speaker_pin, LOW);    // sets the pin off
   delayMicroseconds(pitch2*(1-vol2));        // pauses for 50 microseconds      
}

for (int i=0; i < 10; i++){
   digitalWrite(speaker_pin, HIGH);   // sets the pin on
   delayMicroseconds(pitch5*vol3);        // pauses for 50 microseconds      
   digitalWrite(speaker_pin, LOW);    // sets the pin off
   delayMicroseconds(pitch5*(1-vol3));        // pauses for 50 microseconds      
}


   
}


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.


  pinMode(POL_pin, OUTPUT); //POL pin
  digitalWrite(POL_pin, HIGH);   // set POL 
  pinMode(LE_pin, OUTPUT); //LE pin
  digitalWrite(LE_pin, LOW);   // set LE 
  pinMode(SCK_pin, OUTPUT); //SCK pin
  digitalWrite(SCK_pin, LOW);   // set SCK 
  pinMode(DI1_pin, OUTPUT); //LE pin
  digitalWrite(DI1_pin, LOW);   // set LE 
  pinMode(DI2_pin, OUTPUT); //SCK pin
  digitalWrite(DI2_pin, LOW);   // set SCK 

  pinMode(light_pin, OUTPUT); //SCK pin
  
  pinMode(BUT1_pin, INPUT); 
  digitalWrite(BUT1_pin, HIGH);  
  pinMode(BUT2_pin, INPUT); 
  digitalWrite(BUT2_pin, HIGH);  
  pinMode(BUT3_pin, INPUT); 
  digitalWrite(BUT3_pin, HIGH);  
  pinMode(BUT4_pin, INPUT); 
  digitalWrite(BUT4_pin, HIGH);  

  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  //delay(2000);

  // Clear the buffer.
  display.clearDisplay();
  
bit_sound();

display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5,20);
  display.println("Loading protocol...");
  display.display();
  delay(500);
  display.setCursor(5,29);
  display.println("Press Start");
display.display();

  
  tone(speaker_pin,2000,100);
  delay(100);
tone(speaker_pin,2000,100);
  delay(200);
  while(digitalRead(BUT2_pin) == HIGH);



if (light_flag)   digitalWrite(light_pin, HIGH);   // set SCK 
display.clearDisplay();
 display.drawRect(4, 4, 114,58, 1);
 display.display();
 
 setPad(frog_x,frog_y, 1);
 
}

const byte pad_lookup [8][8] PROGMEM = {
  {4,7,11,14,18,21,25,30},
  {3,8,10,15,19,22,29,26},
  {2,6,12,16,20,23,28,27},
  {1,5,9,13,17,24,32,31},
  {64,60,56,52,48,41,33,34},
  {63,59,53,49,45,42,38,37},
  {62,57,55,50,46,43,36,39},
  {61,58,54,51,47,44,40,35}
};

boolean chip1_state[64];
boolean chip2_state[64];

void update_hvchip() {
  digitalWrite(LE_pin, LOW);   // set LE 
  digitalWrite(SCK_pin, LOW);   // set SCK 

  for(int i=0;i<64;i++) {
    digitalWrite(DI1_pin, chip1_state[i]);   // set LE 
    digitalWrite(DI2_pin, chip2_state[i]);

    digitalWrite(SCK_pin, HIGH);   // set LE 
    digitalWrite(SCK_pin, LOW);   // set LE 
  }

  digitalWrite(LE_pin, HIGH);   // set LE 
  digitalWrite(LE_pin, LOW);   // set LE 
}

void setPad(int x, int y, boolean state) {
  if(x>7) {
    chip1_state[pad_lookup[7-y][15-x]-1] = state;
  } else {
    chip2_state[pad_lookup[y][x]-1]=state;
  }
  
  display.fillRect((x+1)*7-2, (y+1)*7-2, 7,7, state);       
}


/* 
void setPad(int x, int y, boolean state) {
  
  digitalWrite(LE_pin, LOW);   // set LE 
  digitalWrite(SCK_pin, LOW);   // set SCK 
  display.fillRect(x*7-2, y*7-2, 7,7, state);       

  for (int i=0; i < 64; i++) {
    
  }
  for (int i = 1; i <65 ; i++) {
    if (x>8)  { 
      digitalWrite(DI1_pin, (i==pad_lookup[8-y][16-x]));   // set LE 
      digitalWrite(DI2_pin,LOW);
    }
    else
    { 
      digitalWrite(DI2_pin, (i==pad_lookup[y-1][x-1]));   // set LE 
      digitalWrite(DI1_pin,LOW);
      Serial.println(pad_lookup[y-1][x-1]);
    };
             
    digitalWrite(SCK_pin, HIGH);   // set LE 
    digitalWrite(SCK_pin, LOW);   // set LE 
  }; 

  if(sound_flag)   frogger();
  digitalWrite(LE_pin, HIGH);   // set LE 
  digitalWrite(LE_pin, LOW);   // set LE 
}
*/
