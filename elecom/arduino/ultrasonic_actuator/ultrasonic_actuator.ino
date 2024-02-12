#include <NewPing.h>


#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     12  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define RPWM_Output 2
#define LPWM_Output 7
#define LED_PIN     53
#define speedAc     15 //скорость актуатора

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

int distance = 0; 
int distanceLast = 0; //расстояние с прошлого замера
int distanceDelta = 0; //изменение дистанции

void setup() {
  pinMode(RPWM_Output, OUTPUT);
  pinMode(LPWM_Output, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
 
  Serial.begin(115200);

  digitalWrite(LED_PIN, HIGH);
}

void loop() {

  //Замер дистанции до объекта
  distance = sonar.ping_cm(); 
  
  //рассчёт изменения расстояния
  distanceDelta = distance - distanceLast; 
  
  //проверяем необходимые условия для перемещения штока актуатора
  if((abs(distanceDelta) > 2) && (distance < 30) && (distance > 5)){ 
      //запоминаем текущее расстояние для дальнейших рассчётов
      distanceLast = distance; 
      
      //вызываем функцию перемещения штока актуатора
      motion(distanceDelta); 
  }


  delay(1000); 

}

void motion(int le){      
  le *= 10;
  int del = (abs(le)/15.0) * 1000;  
  if(le > 0){
    goForward(del);
  }else if (le < 0){
    goBack(del);
  }
}

void goForward(int del){
  digitalWrite(LED_PIN, LOW);
  analogWrite(LPWM_Output, 255);
  analogWrite(RPWM_Output, 0);
  delay(del);
  analogWrite(LPWM_Output, 0);
  analogWrite(RPWM_Output, 0);
  digitalWrite(LED_PIN, HIGH);
}

void goBack(int del){
  digitalWrite(LED_PIN, LOW);
  analogWrite(LPWM_Output, 0);
  analogWrite(RPWM_Output, 255);
  delay(del);
  analogWrite(LPWM_Output, 0);
  analogWrite(RPWM_Output, 0);
  digitalWrite(LED_PIN, HIGH);
}
