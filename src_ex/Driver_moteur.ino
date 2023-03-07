#include <Arduino.h>
#include <Math.h>


#define mForward_left 6        //PH3
#define mBackward_left 7       //PH4
#define mForward_right 8       //PH5
#define mBackward_right 9      //PH6
uint8_t pwmLeft, pwmRight = 0;
uint8_t *ptr_pwmLeft = &pwmLeft;
uint8_t *ptr_pwmRight = &pwmRight;
uint8_t incomingByte = 0 ;



void setup ()
{
Serial.begin (9600);
pinMode(mBackward_left, OUTPUT);
pinMode(mBackward_right, OUTPUT);
pinMode(mForward_left, OUTPUT);
pinMode(mForward_right, OUTPUT);

angle ( 140 , ptr_pwmLeft,  ptr_pwmRight );
    forward(pwmLeft,pwmRight );
    
}
void loop ()
{
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


void forward (uint8_t L, uint8_t R)
{
  Serial.println (R);
for (float i = 0.1 ; i<1; i = i +0.1)
{
 
  uint8_t newR = R * (i);
  uint8_t newL = L * (i);
  analogWrite (mForward_left, newL);
  analogWrite (mForward_right, newR);
  delay (200);
  Serial.print ("i = ");
  Serial.println (i);
  Serial.print (newL);
  Serial.print (" - ");
  Serial.println (newR);
}
digitalWrite(mBackward_left, LOW);
digitalWrite(mBackward_right, LOW);
analogWrite (mForward_left, L);
analogWrite (mForward_right, R);

Serial.print ("void forward ");
Serial.print ("L = ");
Serial.print (L);
Serial.print ("R = ");
Serial.println (R);




}

void angle (uint8_t angle, uint8_t *ptr_pwmLeft, uint8_t *ptr_pwmRight) //Valeur de l'angle entre 0 et 180, 90 = tout droit
{
  
    
    uint8_t pwmMax = 100, pwmMin = 0; 
    int8_t erreur = angle - 90 ;
    

    if (erreur == 0) {
    *ptr_pwmLeft = pwmMax;
    *ptr_pwmRight = pwmMax;
    }
    else if (erreur < 0)
    {
        *ptr_pwmRight = pwmMax;
        *ptr_pwmLeft = pwmMax + (erreur * pwmMax / 90);
        Serial.println (pwmLeft); 
    }

    else if (erreur > 0 )
    {
        
        *ptr_pwmLeft = pwmMax;
        *ptr_pwmRight = pwmMax - (erreur * pwmMax / 90);
        Serial.println (pwmRight); 
    }
 }
