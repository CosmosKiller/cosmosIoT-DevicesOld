#include <Arduino.h>
#include <ESP8266_ISR_Servo.h>


Servo servo;

const int srv1 = 3;
const int btn1 = 4;
const int btn2 = 5;
const int led1 = 13;

void setup()
{
  Serial.begin(9600);
  servo.attach(srv1);
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  pinMode(led1, OUTPUT);
  servo.write(180);
  delay(1000);
  servo.write(0);
  delay(1000);
  servo.detach();
}

void loop()
{
    if(digitalRead(btn1)==HIGH)
    {
      servo.attach(srv1);
      servo.write(90);
      digitalWrite(led1,HIGH);
      Serial.print("Abierto\n");
      delay(2000);
      servo.write(0);
      digitalWrite(led1, LOW);
      Serial.print("Cerrado\n");
      delay(1000);
      servo.detach();
    }
    else if(digitalRead(btn2)==HIGH)
    {
      servo.attach(srv1);
      servo.write(180);
      digitalWrite(led1,HIGH);
      Serial.print("Abierto\n");
      delay(2000);
      servo.write(0);
      digitalWrite(led1, LOW);
      Serial.print("Cerrado\n");
      delay(1000);
      servo.detach();
    }
    delay(100);
}