#include <Arduino.h>
#include <QTRSensors.h>

QTRSensors qtr;

uint16_t sensorValues[3];
uint8_t angle_value;

// Setup for the qtr sensor
void setup_qtr() {
  Serial.begin(9600);
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2}, 3);

  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW); 
  Serial.println("QTR Setup Done.");
}

// Get qtr sensor angle value range 0 to 180
// Return int8_t
int8_t get_angle(uint8_t nb_moyenne) {
  uint16_t position = 0;
  int16_t sum_angle = 0;
  for (int i = 0; i < nb_moyenne; i++){
    position = qtr.readLineWhite(sensorValues);
    //Serial.print (" position :");
    //Serial.print (position);
    angle_value = map(position, 0, 2000, 0, 180);
    sum_angle =+ angle_value;
    }
    uint16_t avg = sum_angle/nb_moyenne;
    Serial.print("angle: ");
    Serial.print(angle_value);
    return angle_value;
}