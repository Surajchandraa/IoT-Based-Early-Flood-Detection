#include <ThingSpeak.h>
#include <WiFi.h>
const char* ssid="POCO M2 Reloaded";
const char* password="suraj joshi";
WiFiClient client;
const char* myWriteAPIKey="RH7DMFK6BAU2MXCN";
const int echo=12;
const int trig=13;
long myNumber=2120554;
int rainsensorpin=34;
void setup(){
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  pinMode(echo, INPUT);
  pinMode(rainsensorpin,INPUT);
  pinMode(trig, OUTPUT);
  pinMode(2,OUTPUT);
  WiFi.begin(ssid,password);
  while(WiFi.status() !=WL_CONNECTED){
    delay(50);
    Serial.println("connecting");
  }
  Serial.println("connected");
  ThingSpeak.begin(client);
}


void loop() {
 float suraj=waterlevel();
 int joshi=rainsensor();
 ThingSpeak.writeField(myNumber, 1, suraj, myWriteAPIKey);
 ThingSpeak.writeField(myNumber, 2, joshi, myWriteAPIKey);

}
float waterlevel(){
  long duration,cm;
  digitalWrite(trig, LOW);
  delay(2);
  digitalWrite(trig, HIGH);
  delay(10);
  digitalWrite(trig,LOW);
  duration=pulseIn(echo,HIGH);
  cm=duration*0.0344/2;
  Serial.println("distance is=   ");
  Serial.print(cm);
  Serial.print("\n\n");
  return cm;
  
}
int rainsensor(){
  int sensorvalue=analogRead(rainsensorpin);
  if(sensorvalue<3800){
    Serial.println("it is raining outside sensor value is=   ");
    Serial.print(sensorvalue);
    digitalWrite(2,HIGH);
    delay(1000);
    digitalWrite(2,LOW);
    delay(500);
    
  }
  else{
    Serial.println("no rain is there");
    digitalWrite(2,LOW);
  }
  return sensorvalue;
}
