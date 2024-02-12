String mes = " ";
void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
  Serial.println("A im ready");
}

void loop() {
  nextionListen();
  delay(1000);

}

void nextionListen(){
  char incomingChar;
  char end = 0xff;
  String message = "";
  byte countEnd = 0;

  while(Serial1.available()){
    Serial.println("woop woop");
    incomingChar = Serial1.read();
    
    if(incomingChar != end){
        message += incomingChar;
    }
    else{
      countEnd++;
    }
    if(countEnd == 3){
      Serial.println(message);
      //break; 
    }
  } 
}
