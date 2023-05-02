#include <Arduino.h>
#include <HCSR04.h>

#define trig1 22        
#define echo1 24  
#define trig2 23        
#define echo2 25  
UltraSonicDistanceSensor distanceSensor1 (trig1, echo1);   
UltraSonicDistanceSensor distanceSensor2 (trig2, echo2);   

void setup_hcsr() {
  pinMode (trig1, OUTPUT);
  pinMode (echo1, INPUT);
  pinMode (trig2, OUTPUT);
  pinMode (echo2, INPUT);
  
}

float obstacle_hcsr1()
{
  float distance = distanceSensor1.measureDistanceCm();
  return distance;
}

float obstacle_hcsr2()
{
  float distance = distanceSensor2.measureDistanceCm();
  return distance;
}

