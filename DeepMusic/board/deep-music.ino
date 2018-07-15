#include <Keyboard.h>

int sensor_1;
int sensor_2;
int leitor_but;

void setup(){
  Serial.begin(9600);
  Keyboard.begin();
  pinMode(18, INPUT);
  pinMode(19, INPUT);
  pinMode(3, INPUT);
}

void loop(){
  sensor_1 = analogRead(18);
  sensor_2 = analogRead(19);
  leitor_but = digitalRead(3);

  if(sensor_1 > 30){
    Keyboard.write('q');
    delay(60);
  }

  if(sensor_2 > 50){
    Keyboard.write('w');
    delay(60);
  }

  if(leitor_but == HIGH){
    Keyboard.write('r');
    delay(400);
  }
}
