#include <Nextion.h> //подключаем билиотеку экрана
#include <IRremote.h>

#include <Wire.h>
#include <VL53L0X.h>

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

#define RECV_PIN 2

IRrecv irrecv(RECV_PIN);
decode_results results;

void go3PopCallback(void *ptr);
void go4PopCallback(void *ptr);
void go5PopCallback(void *ptr);
void launchingPopCallback(void *ptr);

NexPage page0 = NexPage(0, 0, "page0");

NexPage page2 = NexPage(2, 0, "page2");
NexNumber distance = NexNumber(2, 4, "distance");
NexButton go3 = NexButton(2, 1, "go3");

NexPage page3 = NexPage(3, 0, "page3");
NexNumber tilt_angle = NexNumber(3, 4, "tilt_angle");
NexButton go4 = NexButton(3, 1, "go4");

NexPage page4 = NexPage(4, 0, "page4");
NexNumber width = NexNumber(4, 4, "width");
NexButton go5 = NexButton(4, 1, "go5");

NexPage page5 = NexPage(5, 0, "page5");
NexNumber n0 = NexNumber(5, 1, "n0");
NexNumber n1 = NexNumber(5, 2, "n1");
NexNumber n2 = NexNumber(5, 3, "n2");
NexButton launching = NexButton(5, 4, "launching");

uint32_t dist, tAngle, wid;

char buffer[100] = {0};

NexTouch *nex_listen_list[] = {
  &go3, &go4, &go5, &launching,
  &distance, &tilt_angle, &width,
  &n0, &n1, &n2, &page0, &page2,
  &page5, &page4, &page3,
  NULL
};

void go3PopCallback(void *ptr){
  uint32_t number;

  dbSerialPrintln("go3PopCallback");

  distance.getValue(&number);

  dist = number;
  dbSerialPrintln(dist);
  page3.show();
}

void go4PopCallback(void *ptr){
  uint32_t number;

  dbSerialPrintln("go4PopCallback");

  tilt_angle.getValue(&number);

  tAngle = number;
  dbSerialPrintln(tAngle);
  
  page4.show();
}

void go5PopCallback(void *ptr){
  uint32_t number;

  dbSerialPrintln("go4PopCallback");

  width.getValue(&number);

  wid = number;
  dbSerialPrintln(wid);
  
  page5.show();
}

void launchingPopCallback(void *ptr){
  uint32_t distanceToObject;
  uint32_t tiltAngleFun;
  uint32_t workingWidth;
  
  distanceToObject = ((dist * 10)/15.0)*1000;
  dbSerialPrintln(distanceToObject);

  tiltAngleFun = ((tAngle*10)/35.0)*1000;
  dbSerialPrintln(tiltAngleFun);

  workingWidth = ((((wid*10)-2300)/2.0)/10.0)*1000;
  dbSerialPrintln(workingWidth);

  dbSerialPrintln("Start rabochaa shirina");
  goBack(workingWidth, 13);
  dbSerialPrintln("Start angle");
  goBack(tiltAngleFun, 11);
  dbSerialPrintln("Start distance to object");
  analogWrite(9, 255);
  analogWrite(8, 0);
  analogWrite(7, 255);
  analogWrite(6, 0);
  analogWrite(5, 255);
  analogWrite(4, 0);
  delay(distanceToObject);
  analogWrite(9, 0);
  analogWrite(8, 0);
  analogWrite(7, 0);
  analogWrite(6, 0);
  analogWrite(5, 0);
  analogWrite(4, 0);

  
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

void motionIr(byte number){
  if(number < 0 && number > 9){
    return; 
  }
    
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

VL53L0X sensor;
VL53L0X sensor2;
VL53L0X sensor3;

void setup(void) {
  nexInit();

  go3.attachPop(go3PopCallback);
  go4.attachPop(go4PopCallback);
  go5.attachPop(go5PopCallback);
  launching.attachPop(launchingPopCallback);


  //все порты для управления актуаторами по шим - вывод
  for(byte i = 4; i <=13; i++){
    pinMode(i, OUTPUT);  
  }

  irrecv.enableIRIn();
  dbSerialPrintln("WOOP WOOP");

  pinMode(31, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(35, OUTPUT);

  digitalWrite(31, LOW);
  digitalWrite(33, LOW);
  digitalWrite(35, LOW);

  delay(500);
  Wire.begin();

  pinMode(31, INPUT);
  delay(150);
  sensor.init(true);

  delay(100);
  sensor.setAddress((uint8_t)22);

  pinMode(33, INPUT);
    delay(150);
  sensor2.init(true);
  delay(100);
  sensor2.setAddress((uint8_t)25);


  pinMode(35, INPUT);
    delay(150);
  sensor3.init(true);
  delay(100);
  sensor3.setAddress((uint8_t)28);


  sensor.init();
  sensor2.init();
  sensor3.init();
  
  sensor.setTimeout(500);
  sensor2.setTimeout(500);
  sensor3.setTimeout(500);

  sensor.startContinuous();
  sensor2.startContinuous();
  sensor3.startContinuous();
}

void loop(void) {
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
  motionIr(choiseActuator);

  if(sensor.readRangeContinuousMillimeters() < 60){
      goBack(500,9);
  } else if(sensor.readRangeContinuousMillimeters() > 70){
    goForward(500, 9);  
  }
  
  nexLoop(nex_listen_list);
}

