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
    delay(500);
    if(obstacle_hcsr()>5)
    {
        Serial.print(obstacle_hcsr());
        Serial.print("Right = ");
        Serial.print(pwmRight);
        Serial.print("- Left = ");
        Serial.println(pwmLeft);
        forward(pwmLeft,pwmRight);
    }
    else
    {
        Serial.print(obstacle_hcsr());
        forward(0,0);
    }
    Serial.println("");
}