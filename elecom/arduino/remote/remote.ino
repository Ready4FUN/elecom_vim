 #include <IRremote.h>


int RECV_PIN = 11;
int LED_PIN = 3;
int power = 0;
int state = 1;

IRrecv irrecv(RECV_PIN); 

decode_results results;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  irrecv.enableIRIn();
  
  
  Serial.println("I am ready");
}

void loop() {
  if(irrecv.decode(&results)){
    if(results.value == 0xFFC23D && power < 255){
      state = 1;
      power += 5;
    }
    if(results.value == 0xFF22DD && power > 0){
      state = 0;
      power -= 5;   
    }
    if(results.value == 0xFFFFFFFF && power < 255 && state == 1){
      power += 5;   
    } 
    else if(results.value == 0xFFFFFFFF && power > 0 && state == 0){
      power -= 5;   
    }
    irrecv.resume();
  }
    Serial.println(power);
    analogWrite(LED_PIN, power);
}
