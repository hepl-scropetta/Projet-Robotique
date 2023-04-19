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

float distanceObstacle;
uint8_t angle_read;

#define speed 75
int16_t pwmOutput = 0;

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
    audio_start();
    delay(5000);
    
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
            forward(pwmL, pwmR, LOW, LOW);
        }
        else{
            pwmL = pwmL - abs(pwmOutput)*0.5f;
            pwmR = pwmR - abs(pwmOutput)*3.0f;
            forward(pwmL, LOW, LOW, abs(pwmR));
            }
    }
    // --------------------- Tournant à gauche ---------------------------
    else if(pwmOutput > 0){
        pwmR = abs(pwmOutput) + speed;
        pwmL = speed - (abs(pwmOutput));
        if(pwmL >= 0){
            forward(pwmL, pwmR, LOW, LOW);
        }
        else{
            pwmR = pwmR - abs(pwmOutput)*0.5f;
            pwmL = pwmL - abs(pwmOutput)*3.0f;
            forward(LOW, pwmR, abs(pwmL), LOW);
            }
    }
    else{forward(pwmL, pwmR, LOW, LOW);
    }

    Serial.print(" ");
    Serial.print(pwmL);
    Serial.print(" ");
    Serial.print(pwmR);
    Serial.print(" ");
    

    millisActual = millis();
    if(millisActual - interval > millisPass){
        distanceObstacle = obstacle_hcsr();
        millisPass = millisActual;
    }
    //delay(250);
    /*
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
    */
    Serial.println("");
}