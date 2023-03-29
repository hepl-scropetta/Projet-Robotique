#include <Arduino.h>
#include <Math.h>
#include <qtr.cpp>
#include <Driver_moteur.cpp>
#include <Obstacle_detector.cpp>
#include <SoftwareSerial.h>
#include "bluetooth.h"
#include <PID.cpp>

#define interval 500
uint16_t millisPass = 0;
uint16_t millisActual = 0;

float distanceObstacle;
uint8_t angle_read;

int16_t pwmOutput = 0;

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
    angle_read = get_angle(1);
    angle(angle_read, ptr_pwmLeft,  ptr_pwmRight );
    PID(angle_read);
    pwmOutput = get_pwm();
    uint8_t pwmL = 0;
    uint8_t pwmR = 0;
    if(pwmOutput < 0){
        pwmL = abs(pwmOutput);
        pwmR = 0;
        forward(pwmL, pwmR, false);
    }
    else if(pwmOutput > 0){
        pwmR = abs(pwmOutput);
        pwmL = 0;
        forward(pwmL, pwmR, false);
    }
    else
    {forward(pwmL+40, pwmR+40, false);}
    

    millisActual = millis();
    if(millisActual - interval > millisPass){
        distanceObstacle = obstacle_hcsr();
        millisPass = millisActual;
    }
    //delay(250);

    if(distanceObstacle>15 && distanceObstacle != -1)
    {
        Serial.print(distanceObstacle);
        Serial.print("Right ");
        Serial.print(pwmRight);
        Serial.print("Left ");
        Serial.println(pwmLeft);
        //forward(pwmLeft,pwmRight);
    }
    else if(distanceObstacle>8 && distanceObstacle != -1){
        Serial.print(distanceObstacle);
        Serial.print("Right = ");
        Serial.print(pwmRight/2);
        Serial.print("- Left = ");
        Serial.println(pwmLeft/2);
        //forward(pwmLeft/2,pwmRight/2);
    }
    else
    {
        //forward(0,0);
    }
    Serial.println("");
}