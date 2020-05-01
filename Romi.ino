#include "encoders.h"

#include "lineSensors.h"

//Pin definitions for motor
#define L_PWM_PIN 10
#define L_DIR_PIN 16
#define R_PWM_PIN  9
#define R_DIR_PIN 15

#define BUZZER_PIN 7

#define LEFT_THRESHOLD 200
#define CENTRE_THRESHOLD 200
#define RIGHT_THRESHOLD 200

#define BASE_SPEED 50
#define TURN_SPEED 50


// You may need to change these depending on how you wire
// in your line sensor.
#define LINE_LEFT_PIN A4 //Pin for the left line sensor
#define LINE_CENTRE_PIN A3 //Pin for the centre line sensor
#define LINE_RIGHT_PIN A2 //Pin for the right line sensor

LineSensor line_left(LINE_LEFT_PIN); //Create a line sensor object for the left sensor
LineSensor line_centre(LINE_CENTRE_PIN); //Create a line sensor object for the centre sensor
LineSensor line_right(LINE_RIGHT_PIN); //Create a line sensor object for the right sensor


void setupMotorPins() {
    // Set our motor driver pins as outputs.
  pinMode( L_PWM_PIN, OUTPUT );
  pinMode( L_DIR_PIN, OUTPUT );
  pinMode( R_PWM_PIN, OUTPUT );
  pinMode( R_DIR_PIN, OUTPUT );

  // Set initial direction for l and r
  // low is moving forward
  digitalWrite( L_DIR_PIN, LOW  );
  digitalWrite( R_DIR_PIN, LOW );
}

// Remember, setup only runs once.
void setup() {

  // These two function set up the pin
  // change interrupts for the encoders.
  setupEncoder0();
  setupEncoder1();
  pinMode(LED_BUILTIN, OUTPUT);
  // Initialise your other globals variables
  // and devices.
  line_left.calibrate();
  line_centre.calibrate();
  line_right.calibrate();
  // Initialise the Serial communication
  Serial.begin( 9600 );
  delay(1000);
  Serial.println("***RESET***");

  
}



// Remmeber, loop is called again and again.
void loop() {
    // To store result.
  int l_value; // left sensor
  int c_value; // centre sensor
  int r_value; // right sensor
  // Read analog voltages
  l_value = analogRead( LINE_LEFT_PIN );
  c_value = analogRead( LINE_CENTRE_PIN );
  r_value = analogRead( LINE_RIGHT_PIN );
  int l = l_value - line_left.readCalibrated() + 50;
  int c = c_value - line_centre.readCalibrated() + 50;
  int r = r_value - line_right.readCalibrated() + 50;

  bool onLine;
  bool finish;
  onLine = checkForLine(l,c,r);

  if (onLine){
    blink();
    bangbang(lineprobability(l,c,r));
  }
  else{
    //walk straight may need to change number below
    if (finish){
       beep();
    }
    else{
      bangbang(0.0f);
    }
  }

  // To send data back to your computer.
  // You can open either Serial monitor or plotter.
//  Serial.print( l_value );
//  Serial.print( ", " );
 // Serial.print( c_value );
 // Serial.print( ", " );
  //Serial.print( r_value );
  //Serial.print( "\n" );
  /*
  Serial.print( l );
  Serial.print( ", " );
  Serial.print( c );
  Serial.print( ", " );
  Serial.print( r );
  Serial.print( "\n" );
  */

 volatile int i=0;
  i=i+1;
  Serial.print(i);
      Serial.print("\n");

  delay(50);

  
}
void beep(){
  analogWrite( L_PWM_PIN, 0 );
  analogWrite( R_PWM_PIN, 0 );
  //To implem,ent beep
}

void blink(){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
bool checkForLine(int l, int c, int r){
  if (l>LEFT_THRESHOLD || c>CENTRE_THRESHOLD || r>RIGHT_THRESHOLD){
    return true;
  }
  else{
    return false;
  }
}

//
//if towards-1 means line moves to left, +1 line moves to right, 0, line is middle
float lineprobability(int l,int c,int r){
  float total =  l+c+r;
  float left = l/total;
  float right = r/total;
  float result = left-right;
  /*
    Serial.print("\n left");
  Serial.print(left);
  Serial.print("\nresult");
  Serial.print(result);
    */
  return result;

   
}

void  bangbang(float m){

  int l_power = (m*TURN_SPEED*1)+BASE_SPEED;
  int r_power = (m*TURN_SPEED*-1)+BASE_SPEED;

  if (l_power<0){
    l_power=0;
  }
  if (r_power<0){
    r_power = 0;
  }
  
  analogWrite( L_PWM_PIN, l_power );
  analogWrite( R_PWM_PIN, r_power );

  
}
