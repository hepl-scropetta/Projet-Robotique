#include <Arduino.h>
#include <HCSR04.h>

#define trig 22        
#define echo 23  
UltraSonicDistanceSensor distanceSensor (trig, echo);     

void setup_hcsr() {
  pinMode (trig, OUTPUT);
  pinMode (echo, INPUT);
  Serial.begin (9600);
}

float obstacle_hcsr()
{
  float distance = distanceSensor.measureDistanceCm();
  Serial.print ("distance = ");
  Serial.print (distance);
  Serial.println (" cm");
  return distance;
}

