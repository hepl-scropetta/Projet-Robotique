#include <Arduino.h>
#include <Math.h>
#include <qtr.cpp>
#include <Driver_moteur.cpp>
#include <Obstacle_detector.cpp>
#include <SoftwareSerial.h>
#include "bluetooth.h"
#include <PID.cpp>
#include <Audio.cpp>

#define interval 500
uint16_t millisPass = 0;
uint16_t millisActual = 0;

bool state_obs = 0 ;
float distanceObstacle, distanceObstacle1, distanceObstacle2;
uint8_t angle_read;

#define speed 60
int16_t pwmOutput = 0;
uint8_t percent = 0;
uint8_t *ptr_percent = &percent;


void setup ()
{
    Serial.begin (9600);
    pinMode(mBackward_left, OUTPUT);
    pinMode(mBackward_right, OUTPUT);
    pinMode(mForward_left, OUTPUT);
    pinMode(mForward_right, OUTPUT);

    setup_audio();
    audio_config();
    setup_qtr();
    setup_Driver_moteur();
    setup_hcsr();
    delay(500);
    //audio_start();
    //delay(5000);
    
}
void loop ()
{
    angle_read = get_angle();
    //angle(angle_read, ptr_pwmLeft,  ptr_pwmRight );
    PID(angle_read);
    pwmOutput = get_pwm();
    //pwmOutput = 120;
    int16_t pwmL = speed;
    int16_t pwmR = speed;
    //---------------------- Tournant à droite ----------------------------
    if(pwmOutput < 0){
        pwmL = abs(pwmOutput) + speed;
        pwmR = speed - (abs(pwmOutput));
        if(pwmR >= 0){
            forward(pwmL, pwmR, LOW, LOW,percent);
        }
        else{
            pwmL = pwmL - abs(pwmOutput)*0.5f;
            pwmR = pwmR - abs(pwmOutput)*3.0f;
            forward(pwmL, LOW, LOW, abs(pwmR), percent);
            }
    }
    // --------------------- Tournant à gauche ---------------------------
    else if(pwmOutput > 0){
        pwmR = abs(pwmOutput) + speed;
        pwmL = speed - (abs(pwmOutput));
        if(pwmL >= 0){
            forward(pwmL, pwmR, LOW, LOW, percent);
        }
        else{
            pwmR = pwmR - abs(pwmOutput)*0.5f;
            pwmL = pwmL - abs(pwmOutput)*3.0f;
            forward(LOW, pwmR, abs(pwmL), LOW, percent);
            }
    }
    else{forward(pwmL, pwmR, LOW, LOW, percent);
    }
/*
    Serial.print(" ");
    Serial.print(pwmL);
    Serial.print(" ");
    Serial.print(pwmR);
    Serial.print(" ");
 */   

    millisActual = millis();
    if(millisActual - interval > millisPass){
        if (state_obs == 0)
        {
        distanceObstacle1 = obstacle_hcsr1();
        if (distanceObstacle1==-1)
        {
            distanceObstacle1 = 300;
        }
        state_obs = 1;
        }
        else if (state_obs == 1)
        {
        distanceObstacle2 = obstacle_hcsr2();
        if (distanceObstacle2==-1)
        {
            distanceObstacle2 = 300;
        }
        state_obs = 0;
        }
        
        millisPass = millisActual;
    }
        distanceObstacle = min(distanceObstacle2,distanceObstacle1);
        Serial.print(distanceObstacle);
        Serial.print (" ");
        Serial.print(distanceObstacle1);
        Serial.print (" ");
        Serial.println(distanceObstacle2);
    

    if(distanceObstacle>25 && distanceObstacle != -1)
    {
        *ptr_percent = 100;
    }

    else if(distanceObstacle > 15 && distanceObstacle != -1)
    {
        *ptr_percent = 75;
    }


    else if (distanceObstacle > 12 && distanceObstacle != -1){
        *ptr_percent = 65;
    }
    else if (distanceObstacle != -1)
    {
        
        *ptr_percent = 0;
        Serial.print("stop ");
        
    }
    
    
    
}