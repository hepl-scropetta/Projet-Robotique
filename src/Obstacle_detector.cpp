#include <Arduino.h>
#include <HCSR04.h>

#define trig 22        
#define echo 23  
UltraSonicDistanceSensor distanceSensor (trig, echo);     

void obstacle ()
{
  float distance = distanceSensor.measureDistanceCm();
  Serial.print ("distance = ");
  Serial.print (distance);
  Serial.println (" cm");
}

void setup() {
  // put your setup code here, to run once:
pinMode (trig, OUTPUT);
pinMode (echo, INPUT);
Serial.begin (9600);
}

void loop() {
  
  // put your main code here, to run repeatedly:
obstacle ();
delay (500);
}


