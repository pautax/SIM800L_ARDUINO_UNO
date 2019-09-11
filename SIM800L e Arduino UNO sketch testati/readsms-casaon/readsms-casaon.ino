#include "SoftwareSerial.h";
 
//Definiamo una SoftwareSerial con i pin 8 ed 7 che devono essere collegati rispettivamente al TX ed RX del SIM800L
SoftwareSerial SIM800L(3, 2);
 
String lastLine;
int str_len;
char lastCharRead;
//int CASA = 10;
 
void setup() {
//Inizializziamo la seriale dell'arduino verso il computer (fisica)
Serial.begin(57600);
while(!Serial);
 
//Inizializziamo la seriale dell'arduino verso il SIM800L (software)
SIM800L.begin(57600);
delay(1000);
 
Serial.println("Serial ok!");
delay(10000);
SIM800L.print("AT+CMGF=1\r\n");
delay(1000);
SIM800L.print("AT+CNMI=1,2,0,0,0\r\n");
delay(1000);
Serial.println("Inizializzato, in attesa di SMS");
//pinMode(CASA, OUTPUT);
}
char currentLine[500] = "";
int currentLineIndex = 0;
 
bool nextIsMessage = false;
 
void loop() {
 
  if(SIM800L.available()){
    lastCharRead = SIM800L.read();
 
    if(lastCharRead == '\r' || lastCharRead == '\n'){
      lastLine = String(currentLine);
      Serial.println(lastLine);
  
      if(lastLine.startsWith("+CMT:")){
        nextIsMessage = true; 
      } else if (lastLine.length() > 0) {
        if(nextIsMessage) {
          if(lastLine.startsWith("CASA:") >= 0){
            if(lastLine.endsWith("ON")) {
              //digitalWrite(CASA, HIGH);
            }else if(lastLine.endsWith("OFF")) {
              //digitalWrite(CASA, LOW);
            }else {
              Serial.println("Per l'opzione CASA scegliere tra ON e OFF");
            }
          }
 
          nextIsMessage = false;
          SIM800L.println("AT+CMGD=1,4\r\n");
          delay(500);
          Serial.println("Messaggio cancellato!");
        }
 
      }
 
      for( int i = 0; i < sizeof(currentLine); ++i ) {
        currentLine[i] = (char)0;
      }
      currentLineIndex = 0;
    } else {
      currentLine[currentLineIndex++] = lastCharRead;
  }
}
}
