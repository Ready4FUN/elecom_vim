#include <LiquidCrystal.h>
#include <NewPing.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define VCC 53

#define TRIGGER_PIN  51  
#define ECHO_PIN     51  
#define MAX_DISTANCE 400

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  pinMode(VCC, OUTPUT);
  digitalWrite(VCC, HIGH);
  lcd.begin(16, 2);               // start the library
  lcd.setCursor(0,0);             // set the LCD cursor   position 
  Serial.begin(115200);
}


void loop() {
  Serial.println(sonar.ping_cm());
  lcd.print(sonar.ping_cm());
  delay(500);
  lcd.clear();

}
