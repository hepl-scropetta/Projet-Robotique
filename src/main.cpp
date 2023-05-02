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
uint16_t millisFirstZero = 0;

float distanceObstacle;
uint8_t angle_read;
int angle_demi;

#define speed 60
int16_t pwmOutput = 0;

void setup ()
{
    Serial.begin(9600);
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
    pwmOutput = PID(angle_read);
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
    // --------------------- Tournant à gauche ----------------------------
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

    //---------------------- Detection fin de parcour ---------------------

    if(pwmOutput == 0 && millisFirstZero == 0){
        millisFirstZero = millis();
        }
    if(pwmOutput != 0){
        millisFirstZero = 0;
    }
    uint16_t timeMillis = millis();
    if (millisFirstZero != 0 && (timeMillis - 100) >= millisFirstZero){
        //------------- Demi-tour --------------
        forward(0, 0, LOW, LOW);
        delay(200);
        millisFirstZero = 0;
        angle_demi = 0;
        while(angle_demi < 5)
        {
            angle_demi = PID(get_angle());
            forward(LOW, 100, 130, LOW);
        }
    }

    //------------------------- Debug Serial ------------------------------
    Serial.print(0);
    Serial.print(" ");
    Serial.print(pwmOutput);
    Serial.print(" ");
    Serial.print(pwmL);
    Serial.print(" ");
    Serial.print(pwmR);
    Serial.println("");
    
    //---------------------- Dectection obstacle --------------------------
    millisActual = millis();
    if(millisActual - interval > millisPass){
        distanceObstacle = obstacle_hcsr();
        millisPass = millisActual;
    }  
}