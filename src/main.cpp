#include <Arduino.h>
#include <Math.h>
#include <qtr.cpp>
#include <Driver_moteur.cpp>
#include <Obstacle_detector.cpp>
#include <SoftwareSerial.h>
#include "bluetooth.h"


void setup ()
{
    Serial.begin (9600);
    pinMode(mBackward_left, OUTPUT);
    pinMode(mBackward_right, OUTPUT);
    pinMode(mForward_left, OUTPUT);
    pinMode(mForward_right, OUTPUT);

    setup_qtr();
    setup_Driver_moteur();
    setup_hcsr();
}
void loop ()
{
    angle(get_angle(20), ptr_pwmLeft,  ptr_pwmRight );
    if(true){
        forward(pwmLeft,pwmRight);
    }
    else{
        forward(0,0);
    }
    Serial.println("");
}