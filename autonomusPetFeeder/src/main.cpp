#include <Arduino.h>
#include <ESP8266_ISR_Servo.h>


ESP8266_ISR_Servo servo;

const int srv1 = 3;
const int btn1 = 4;
const int btn2 = 5;
const int led1 = 13;

int8_t srv1_id = servo.setupServo(srv1);

void setup()
{
  Serial.begin(9600);
  servo.enable(srv1_id);
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  pinMode(led1, OUTPUT);
  servo.setPosition(srv1_id, 180);
  delay(1000);
  servo.setPosition(srv1_id, 0);
  delay(1000);
  servo.disable(srv1_id);
}

void loop()
{
  servo.enable(srv1_id);

  if(digitalRead(btn1)==HIGH)
  {
    servo.setPosition(srv1_id, 90);
  }
  else if(digitalRead(btn2)==HIGH)
  {
    servo.setPosition(srv1_id, 180);
  }

  servo.setPosition(srv1_id, 90);
  digitalWrite(led1,HIGH);
  Serial.print("Abierto\n");
  delay(2000);
  servo.setPosition(srv1_id, 0);
  digitalWrite(led1, LOW);
  Serial.print("Cerrado\n");
  delay(1000);
  servo.disable(srv1_id);
  delay(100);
}