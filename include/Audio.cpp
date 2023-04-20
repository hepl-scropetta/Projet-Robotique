#include <Arduino.h>

byte mario [6] = {0x7E, 0x04, 0x03, 0x00, 0x02, 0xEF};//6 sec
byte Config [6] = {0x7E, 0x04, 0x03, 0x00, 0x03, 0xEF};//5 sec
byte Motor [6] = {0x7E, 0x04, 0x03, 0x00, 0x05, 0xEF};
byte MotorGo [6] = {0x7E, 0x04, 0x03, 0x00, 0x04, 0xEF};
byte Drift [6] = {0x7E, 0x04, 0x03, 0x00, 0x06, 0xEF};
byte Obstacle [6] = {0x7E, 0x04, 0x03, 0x00, 0x07, 0xEF};
bool Mode = 0;

byte Time [4] = {0x7E, 0x02, 0x50, 0xEF};
byte play [4] = {0x7E, 0x02, 0x0D, 0xEF};
byte next [4] = {0x7E, 0x02, 0x01, 0xEF};
byte VolumeSetup [5] = {0x7E , 0x03 , 0x06 , 0x12, 0xEF};
byte Volume [5] = {0x7E , 0x03 , 0x06 , 0x17, 0xEF};

void DefineMode (bool m)
{
  if (m)
  {
    Mode = 0;
  }
  else 
  {
    Mode = 1;
  }
} 

void GetTime ()
{
  Serial1.write(Time, sizeof(Time));
  while(Serial1.available())
  {
    uint16_t audioTime = Serial1.read();
    Serial.print (audioTime);
  }
  Serial.print (Serial1.read());
  
}

void setup_volume() {
  // put your setup code here, to run once:

Serial1.write(Volume, sizeof(Volume));
}


void setup_audio() {
  // put your setup code here, to run once:
Serial1.begin (9600);
delay (50);
Serial1.write(VolumeSetup, sizeof(VolumeSetup));
}


void audio_config ()
{
 Serial1.write(Config, sizeof(Config));   
}

void audio_start ()
{
  if (Mode == 1)
  {
    Serial1.write(mario, sizeof(mario));
  }
}

void audio_motor_go ()
{
  if (Mode == 1)
  {
    Serial1.write(MotorGo, sizeof(MotorGo));
  }
}

void audio_motor ()
{
  if (Mode == 1)
  {
    Serial1.write(Motor, sizeof(Motor));
  }
}

void audio_drift ()
{
  if (Mode == 1)
  {
    Serial1.write(Drift, sizeof(Drift));
  }
}

void audio_obstacle ()
{
  if (Mode == 1)
  {
    Serial1.write(Obstacle, sizeof(Obstacle));
  }
}

