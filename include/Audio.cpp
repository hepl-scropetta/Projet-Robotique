#include <Arduino.h>

byte mario [6] = {0x7E, 0x04, 0x03, 0x00, 0x02, 0xEF};//6 sec
byte Config [6] = {0x7E, 0x04, 0x03, 0x00, 0x03, 0xEF};//5 sec

byte play [4] = {0x7E, 0x02, 0x0D, 0xEF};
byte next [4] = {0x7E, 0x02, 0x01, 0xEF};
byte Volume [5] = {0x7E , 0x03 , 0x06 , 0x12, 0xEF};

void setup_audio() {
  // put your setup code here, to run once:
Serial1.begin (9600);
delay (50);
Serial1.write(Volume, sizeof(Volume));
}


void audio_config ()
{
 Serial1.write(Config, sizeof(Config));   
}

void audio_start ()
{
    Serial1.write(mario, sizeof(mario));
}