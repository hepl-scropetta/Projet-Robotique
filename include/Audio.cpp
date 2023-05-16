#include <Arduino.h>
byte vache [6] = {0x7E, 0x04, 0x03, 0x00, 0x01, 0xEF};//6 sec
byte mario [6] = {0x7E, 0x04, 0x03, 0x00, 0x02, 0xEF};//6 sec
byte Config [6] = {0x7E, 0x04, 0x03, 0x00, 0x03, 0xEF};//5 sec
byte Reverse [6] = {0x7E, 0x04, 0x03, 0x00, 0x04, 0xEF};//5 sec
byte Obstacle [6] = {0x7E, 0x04, 0x03, 0x00, 0x05, 0xEF};//5 sec



byte play [4] = {0x7E, 0x02, 0x0D, 0xEF};
byte next [4] = {0x7E, 0x02, 0x01, 0xEF};
byte Volume22 [5] = {0x7E , 0x03 , 0x06 , 0x22, 0xEF};
byte Volume15 [5] = {0x7E , 0x03 , 0x06 , 0x15, 0xEF};

 
void setup_audio() {
  // put your setup code here, to run once:
Serial1.begin (9600);
delay (50);
}


void audio_config ()
{
    Serial1.write(Volume15, sizeof(Volume15));
    Serial1.write(Config, sizeof(Config));   
}

void audio_start ()
{
    Serial1.write(Volume15, sizeof(Volume15));
    Serial1.write(mario, sizeof(mario));
}

void audio_Reverse ()
{
    Serial1.write(Volume22, sizeof(Volume22));
    Serial1.write(Reverse, sizeof(Reverse));
}

void audio_Obstacle ()
{   
    Serial1.write(Volume22, sizeof(Volume22));
    Serial1.write(Obstacle, sizeof(Obstacle));
}
