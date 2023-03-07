#include <Arduino.h>

#define mForward_left 15        //PH3
#define mBackward_left 16       //PH4
#define mForward_right 17       //PH5
#define mBackward_right 18      //PH6
uint8_t L, R = 0;
uint8_t incomingByte = 0 ;


void setup ()
{
Serial.begin (9600);
pinMode(mBackward_left, OUTPUT);
pinMode(mBackward_right, OUTPUT);
pinMode(mForward_left, OUTPUT);
pinMode(mForward_right, OUTPUT);
}
void loop ()
{
 if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    angle (incomingByte);
    }
}
void forward (uint8_t L, uint8_t R)
{
digitalWrite(mBackward_left, LOW);
digitalWrite(mBackward_right, LOW);
analogWrite (mForward_left, L);
analogWrite (mForward_right, R);



}

void angle (uint8_t angle) //Valeur de l'angle entre 0 et 180, 90 = tout droit
{
    uint8_t pwmMax = 255, pwmMin = 0; 
    uint8_t erreur = angle - 90 ;

    if (erreur == 0) {
    pwmLeft = pwmMax;
    pwmRight = pwmMax;

    else if (erreur < 0)
    {
        pwmRight = pwmMax;
        pwmLeft = pwmMax - (erreur * pwmMax / 90);
    }

    else if (erreur > 0 )
    {
        
        pwmLeft = pwmMax;
        pwmRight = pwmMax - (erreur * pwmMax / 90);
    }

    pwmLeft = max(pwmLeft, 0);
    pwmLeft = min(pwmLeft, pwmMax);
    pwmRight = max(pwmRight, 0);
    pwmRight = min(pwmRight, pwmMax);

    return (pwmLeft, pwmRight);
}
