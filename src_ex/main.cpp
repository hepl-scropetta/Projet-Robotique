#include <Arduino.h>
#include <Math.h>
#include <qtr.cpp>
#include <HCSR04.h>

#define mForward_left 6        //PH3
#define mBackward_left 7       //PH4
#define mForward_right 8       //PH5
#define mBackward_right 9      //PH6
uint8_t pwmLeft, pwmRight = 0;
uint8_t *ptr_pwmLeft = &pwmLeft;
uint8_t *ptr_pwmRight = &pwmRight;
uint8_t incomingByte = 0 ;

void forward (uint8_t L, uint8_t R)
{
    Serial.print(" ");
    Serial.print(L);
    Serial.print(" - ");
    Serial.println(R);
    if(L==0 && R==0){
        for (float i = 0.1 ; i<1; i = i +0.1)
        {
            uint8_t newR = R * (i);
            uint8_t newL = L * (i);
            analogWrite (mForward_left, newL);
            analogWrite (mForward_right, newR);
            delay (200);
        }
    }
digitalWrite(mBackward_left, LOW);
digitalWrite(mBackward_right, LOW);
analogWrite (mForward_left, L);
analogWrite (mForward_right, R);
}

//Valeur de l'angle entre 0 et 180, 90 = tout droit
void angle (uint8_t angle, uint8_t *ptr_pwmLeft, uint8_t *ptr_pwmRight){
    uint8_t pwmMax = 70, pwmMin = 0; 
    int8_t erreur = angle - 90 ;
    if (erreur <= 1 && erreur >= -1) {
    *ptr_pwmLeft = pwmMax;
    *ptr_pwmRight = pwmMax;
    }
    else if (erreur < 0)
    {
        uint8_t ratio = map(erreur, -1, -90, 1, 10);
        *ptr_pwmRight = pwmMax*(1 + (ratio/10));
        *ptr_pwmLeft = (pwmMax + (erreur * pwmMax / 90)/2);
    }
    else if (erreur > 0 )
    {
        uint8_t ratio = map(erreur, 1, 90, 1, 10);
        *ptr_pwmLeft = pwmMax*(1 + (ratio/10));
        *ptr_pwmRight = (pwmMax - (erreur * pwmMax / 90)/2);
    }
}

void setup ()
{
Serial.begin (9600);
pinMode(mBackward_left, OUTPUT);
pinMode(mBackward_right, OUTPUT);
pinMode(mForward_left, OUTPUT);
pinMode(mForward_right, OUTPUT);

setup_qtr();

//angle ( 140 , ptr_pwmLeft,  ptr_pwmRight );
//    forward(pwmLeft,pwmRight );
    
}
void loop ()
{
    angle(get_angle(), ptr_pwmLeft,  ptr_pwmRight );
    forward(pwmLeft,pwmRight );

/*  //analogWrite(mForward_left, 100); 
 while(Serial.available() > 0) {
    // read the incoming byte:
    uint8_t myNumber = Serial.parseInt();
    incomingByte = Serial.read();
    Serial.println (myNumber);
    angle (90, ptr_pwmLeft,  ptr_pwmRight );
    forward(pwmLeft,pwmRight );
    delay(100);
    }
   */ 
    
}