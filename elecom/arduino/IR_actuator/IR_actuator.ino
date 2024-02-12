#include <IRremote.h>

#define RECV_PIN 2

#define R_width  13
#define L_width  12

#define R_angle  11
#define L_angle  10

#define R_left_height   9
#define L_left_height  8

#define R_middle_height   7
#define L_middle_height  6

#define R_right_height  5
#define L_right_height  4

#define LED_PIN  53

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup() {
  Serial.begin(9600);
  pinMode(R_width, OUTPUT);
  pinMode(L_width, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  irrecv.enableIRIn();
  Serial.println("I am ready");
  //digitalWrite(LED_PIN, HIGH);
}

void loop() {
  static byte  choiseActuator;
  if(irrecv.decode(&results)){
    switch(results.value){
      case 0xFF6897:
          choiseActuator = R_width;
          break;
      case 0xFF9867:
          choiseActuator = R_angle;
          break;
      case 0xFFB04F:
          choiseActuator = R_left_height;
          break;
      case 0xFF30CF:
          choiseActuator = R_middle_height;
          break;
      case 0xFF18E7:
          choiseActuator = R_right_height;
          break;
    }
  }
  motion(choiseActuator);
}

void goForward(int del, byte number){
  //digitalWrite(LED_PIN, LOW);
  analogWrite(number - 1, 255);
  analogWrite(number, 0);
  delay(del);
  analogWrite(number - 1, 0);
  analogWrite(number, 0);
  //digitalWrite(LED_PIN, HIGH);
}

void goBack(int del, byte number){
  //digitalWrite(LED_PIN, LOW);
  analogWrite(number - 1, 0);
  analogWrite(number, 255);
  delay(del);
  analogWrite(number - 1 , 0);
  analogWrite(number, 0);
  //digitalWrite(LED_PIN, HIGH);
}


void motion(byte number){
  
  if(irrecv.decode(&results)){
    Serial.println("have");
   if(results.value == 0xFFC23D){
      Serial.println("Forward");
      goForward(3000, number);
    } 
    else if(results.value == 0xFF22DD){
      Serial.print("Back");
      goBack(3000, number);
    }
    irrecv.resume();
  }
}
