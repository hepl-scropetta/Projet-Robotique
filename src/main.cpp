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

bool state_obs = 0 ;
float distanceObstacle, distanceObstacle1, distanceObstacle2;
uint8_t angle_read;
int angle_demi;

#define speed 60
int16_t pwmOutput = 0;
uint8_t percent = 0;
uint8_t *ptr_percent = &percent;


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
            forward(pwmL, pwmR, LOW, LOW, percent);
        }
        else{
            pwmL = pwmL - abs(pwmOutput)*2.5f;
            pwmR = pwmR - abs(pwmOutput)*1.0f;
            forward(pwmL, LOW, LOW, abs(pwmR), percent);
            }
    }
    // --------------------- Tournant à gauche ----------------------------
    else if(pwmOutput > 0){
        pwmR = abs(pwmOutput) + speed;
        pwmL = speed - (abs(pwmOutput));
        if(pwmL >= 0){
            forward(pwmL, pwmR, LOW, LOW, percent);
        }
        else{
            pwmR = pwmR - abs(pwmOutput)*2.5f;
            pwmL = pwmL - abs(pwmOutput)*1.0f;
            forward(LOW, pwmR, abs(pwmL), LOW, percent);
            }
    }
    else{forward(pwmL, pwmR, LOW, LOW, percent);
    }

    //---------------------- Detection fin de parcour ---------------------

    if(pwmOutput == 0 && millisFirstZero == 0){
        millisFirstZero = millis();
        }
    if(pwmOutput != 0){
        millisFirstZero = 0;
    }
    uint16_t timeMillis = millis();
    if (millisFirstZero != 0 && (timeMillis - 40) >= millisFirstZero){
        //------------- Demi-tour --------------
        forward(0, 0, LOW, LOW, 0);
        delay(1000);
        millisFirstZero = 0;
        angle_demi = 0;
        while(angle_demi < 5)
        {
            angle_demi = PID(get_angle());
            forward(LOW, 120, 160, LOW, 100);
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