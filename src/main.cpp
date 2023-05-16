#include <Arduino.h>
#include <Math.h>
#include <qtr.cpp>
#include <Driver_moteur.cpp>
#include <Obstacle_detector.cpp>
#include <SoftwareSerial.h>
#include "bluetooth.h"
#include <PID.cpp>
#include <Audio.cpp>
#include <Servo.h>

#define interval 500
uint16_t millisPass = 0;
uint16_t millisActual = 0;
uint16_t millisFirstZero = 0;

uint16_t millisPassLed = 0;
uint16_t millisActualLed = 0;

bool state_obs = 0 ;
float distanceObstacle, distanceObstacle1, distanceObstacle2;
bool obstacle_stop = false;
uint16_t obstacle_stop_millis = 0;
uint8_t angle_read;
int angle_demi;
bool half_path_completed = false;

#define speed 60
int16_t pwmOutput = 0;
uint8_t percent = 0;
uint8_t *ptr_percent = &percent;

Servo servo_main;
Servo servo_sec;

#define LED_PIN 26
int intervalLed = 1000;

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
    servo_main.attach(4);
    servo_sec.attach(3);
    servo_main.write(90);
    servo_sec.write(130);
    delay(1000);
    servo_main.write(130);
    servo_sec.write(170);
    delay(500);
    servo_main.write(90);
    servo_sec.write(130);
    pinMode(LED_PIN, OUTPUT);
}
void loop ()
{
    angle_read = get_angle();
    intervalLed = 10;
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
    if (millisFirstZero != 0 && (timeMillis - 70) >= millisFirstZero){
        //------------- Demi-tour --------------
        forward(0, 0, 50, 50, 100);
        delay(200);
        forward(0, 0, 0, 0, 100);
        servo_main.write(130);
        servo_sec.write(170);
        delay(1000);
        servo_main.write(180);
        servo_sec.write(130);
        delay(500);
        millisFirstZero = 0;
        angle_demi = 0;
        if(half_path_completed){
            while (true);
        }
        audio_Reverse();
        delay(2000);
        while(angle_demi < 5)
        {
            angle_demi = PID(get_angle());
            intervalLed = 1000;
            forward(LOW, 150, 170, LOW, 100);
            half_path_completed = true;
        }
        servo_main.write(90);
        servo_sec.write(130);
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
        *ptr_percent = 45;
    }
    else if (distanceObstacle != -1)
    {
        *ptr_percent = 0;
        if(!obstacle_stop){
            obstacle_stop_millis = millis();
            obstacle_stop = true;
        }
        intervalLed = 500;
    }

    //---------------------- Obstacle problem  --------------------------
    millisActual = millis();
    if(obstacle_stop){
        if(millisActual - 2000 > obstacle_stop_millis){
            obstacle_stop_millis = millisActual;
            audio_Obstacle();
            obstacle_stop = false;
        }
    }

    //---------------------- Clinotement led --------------------------
    millisActual = millis();
    if(millisActual - intervalLed > millisPassLed){
        millisPassLed = millisActual;
        if(digitalRead(LED_PIN)){
            digitalWrite(LED_PIN, LOW);
        }
        else{
            digitalWrite(LED_PIN, HIGH);
        }
    }
}