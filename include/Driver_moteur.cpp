#include <Arduino.h>

#define mForward_left 6        //PH3
#define mBackward_left 7       //PH4
#define mForward_right 8       //PH5
#define mBackward_right 9      //PH6
uint8_t pwmLeft, pwmRight = 0;
uint8_t *ptr_pwmLeft = &pwmLeft;
uint8_t *ptr_pwmRight = &pwmRight;
uint8_t incomingByte = 0 ;
uint8_t L, R = 0;

void setup_Driver_moteur ()
{
    Serial.begin (9600);
    pinMode(mBackward_left, OUTPUT);
    pinMode(mBackward_right, OUTPUT);
    pinMode(mForward_left, OUTPUT);
    pinMode(mForward_right, OUTPUT);
}

//Valeur de l'angle entre 0 et 180, 90 = tout droit
void angle (uint8_t angle, uint8_t *ptr_pwmLeft, uint8_t *ptr_pwmRight){

    uint8_t pwmMax = 70, pwmMin = 0; 
    int8_t erreur = angle - 90 ;
    if (erreur <= 5 && erreur >= -5) {
    *ptr_pwmLeft = pwmMax*0.7;
    *ptr_pwmRight = pwmMax*0.7;
    }
    else if (erreur < 0)
    {
        if(erreur > -45){
            *ptr_pwmRight = (pwmMax - (erreur * (pwmMax - pwmMin) / 90));
            *ptr_pwmLeft = (pwmMax  + (erreur * (pwmMax - pwmMin) / 90))*0;
        }
        else{
            *ptr_pwmRight = (pwmMax - (erreur * (pwmMax - pwmMin) / 90))/*1.3f*/;
            *ptr_pwmLeft = (pwmMax  + (erreur * (pwmMax - pwmMin) / 90))*0;
        }
        
    }
    else if (erreur > 0 )
    {
        if(erreur < 45){
            *ptr_pwmLeft = (pwmMax  + (erreur * (pwmMax - pwmMin) / 90));
            *ptr_pwmRight = (pwmMax - (erreur * (pwmMax - pwmMin) / 90))*0;
        }
        else{
            *ptr_pwmLeft = (pwmMax  + (erreur * (pwmMax - pwmMin) / 90))/*1.3f*/;
            *ptr_pwmRight = (pwmMax - (erreur * (pwmMax - pwmMin) / 90))*0;
        }
        
    }
}


//  forward(pwmLeft,pwmRight );
void forward (uint8_t L, uint8_t R, uint8_t rev_L, uint8_t rev_R, uint8_t percent)
{
    L = (L * percent) / 100;
    R = (R * percent) / 100;
    rev_L = (rev_L * percent) / 100;
    rev_R = (rev_R * percent) / 100;

    /*
    if(L==0 && R==0){
        for (float i = 0.1 ; i<1; i = i +0.1)
        {
            uint8_t newR = R * (i);
            uint8_t newL = L * (i);
            analogWrite (mForward_left, newL);
            analogWrite (mForward_right, newR);
            //delay (200);
        }
    }
    */
analogWrite (mForward_left, L);
analogWrite (mForward_right, R);
analogWrite(mBackward_left, rev_L);
analogWrite(mBackward_right, rev_R);
}

void stop ()
{
    forward (0,0,0,0,0);
}