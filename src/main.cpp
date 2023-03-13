#include <Arduino.h>
#include <Math.h>
#include <qtr.cpp>
#include <Driver_moteur.cpp>
#include <HCSR04.h>


void setup ()
{
Serial.begin (9600);
pinMode(mBackward_left, OUTPUT);
pinMode(mBackward_right, OUTPUT);
pinMode(mForward_left, OUTPUT);
pinMode(mForward_right, OUTPUT);

setup_qtr();
setup_Driver_moteur();

//angle ( 140 , ptr_pwmLeft,  ptr_pwmRight );
//    forward(pwmLeft,pwmRight );
    
}
void loop ()
{
    angle(get_angle(20), ptr_pwmLeft,  ptr_pwmRight );
    forward(pwmLeft,pwmRight );

/*  //analogWrite(mForward_left, 100); 
 while(Serial.available() > 0) {
    // read the incoming byte:
    uint8_t myNumber = Serial.parseInt();
    incomingByte = Serial.read();
    Serial.println (myNumber);
    angle (90, ptr_pwmLeft,  ptr_pwmRight );
    forward(pwmLeft,pwmRight );
    delay(100);
    }
   */ 
    
}