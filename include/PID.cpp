// This alternate version of the code does not require
// atomic.h. Instead, interrupts() and noInterrupts() 
// are used. Please use this code if your 
// platform does not support ATOMIC_BLOCK.

#include <Arduino.h>

volatile int posi = 0; // specify posi as volatile: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/
long prevT = 0;
float eprev = 0;
float eintegral = 0;
float u = 0;

void PID(uint8_t angle_read) {

  // set target position
  int target = 90;
  // PID constants
  float kp = 1.1f;
  float kd = 0.0f;
  float ki = 0.0f;

  // time difference
  long currT = micros();
  float deltaT = ((float) (currT - prevT))/( 1.0e6 );
  prevT = currT;

  // error
  int e = target - angle_read;

  // derivative
  float dedt = (e-eprev)/(deltaT);

  // integral
  eintegral = eintegral + e*deltaT;

  // control signal
  u = kp*e + kd*dedt + ki*eintegral;
  
  // store previous error
  eprev = e;

  Serial.print(0);
  Serial.print(" ");
  //Serial.print(e);
  //Serial.print(" ");
}

int16_t get_pwm(){
  int16_t pwmOutput = map(u, -90, 90, -90, 90);
  Serial.print(pwmOutput);
  return pwmOutput;
}