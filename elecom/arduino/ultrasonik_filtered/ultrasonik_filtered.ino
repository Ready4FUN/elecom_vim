#include <NewPing.h>

// сонар
#define ECHO 13
#define TRIG 12

NewPing sonar(TRIG, ECHO, 90);

float dist_3[3] = {0.0, 0.0, 0.0};   // массив для хранения трёх последних измерений
float middle, dist, dist_filtered;
float k;
byte i, delta;
unsigned long dispIsrTimer, sensTimer;

void setup() {
  Serial.begin(9600);
  pinMode(11, OUTPUT);
  
  digitalWrite(11, HIGH);
}

void loop() {
  static uint16_t lambda = 0;
  static bool readyToGo = false;
  if (millis() - sensTimer > 50) {                          // измерение и вывод каждые 50 мс
    
    // счётчик от 0 до 2
    // каждую итерацию таймера i последовательно принимает значения 0, 1, 2, и так по кругу
    if (i > 1) i = 0;
    else i++;

    dist_3[i] = (float)sonar.ping() / 57.5;                 // получить расстояние в текущую ячейку массива
    dist = middle_of_3(dist_3[0], dist_3[1], dist_3[2]);    // фильтровать медианным фильтром из 3ёх последних измерений

    delta = abs(dist_filtered - dist);                      // расчёт изменения с предыдущим
    if (delta > 1) k = 0.5;                                 // если большое - резкий коэффициент
    else k = 0.1;                                           // если маленькое - плавный коэффициент

    dist_filtered = dist * k + dist_filtered * (1 - k);    // фильтр "бегущее среднее"
    
    Serial.println(dist_filtered); 
    sensTimer = millis();                                   // сбросить таймер
  }

  lambda = dist - dist_filtered;
  
  //инициализация движения штока актуатора
  if((abs(lambda) > 2) && (dist < 90) && (dist > 20) && !readyToGo){
    lambda *= 10;
    int del = (abs(lambda)/15.0) * 1000;
    if(lambda > 0){
      goBack(del);  
    }else if(lambda < 0){
      goForward(del);  
    }
  }
}

// медианный фильтр из 3ёх значений
float middle_of_3(float a, float b, float c) {
  if ((a <= b) && (a <= c)) {
    middle = (b <= c) ? b : c;
  }
  else {
    if ((b <= a) && (b <= c)) {
      middle = (a <= c) ? a : c;
    }
    else {
      middle = (a <= b) ? a : b;
    }
  }
  return middle;
}

//движение штока актуатора
void goForward(int del){
  digitalWrite(LED_PIN, LOW);
  analogWrite(LPWM_Output, 255);
  analogWrite(RPWM_Output, 0);
  delay(del);
  analogWrite(LPWM_Output, 0);
  analogWrite(RPWM_Output, 0);
  digitalWrite(LED_PIN, HIGH);
}
//движение штока актуатора
void goBack(int del){
  digitalWrite(LED_PIN, LOW);
  analogWrite(LPWM_Output, 0);
  analogWrite(RPWM_Output, 255);
  delay(del);
  analogWrite(LPWM_Output, 0);
  analogWrite(RPWM_Output, 0);
  digitalWrite(LED_PIN, HIGH);
}
