#include <Arduino.h>

#include <ThingSpeak.h>
#include <WiFi.h>
#include <DHT.h>


#define DHTPIN 14
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const char* ssid="POCO M2 Reloaded";
const char* password="suraj best";
WiFiClient client;
const char* myWriteAPIKey="Use Thingspeak api key";
const int echo=12;
const int trig=13;
long myNumber=2120554;
int rainsensorpin=34;

float waterlevel();
int rainsensor();
float getHumidity();
int gettemp();
float calculateFloodPossibility(float waterLevel, int rain, float humidity, int temperature);


const float THRESHOLD_WATER_LEVEL = 50.0; // Define a threshold water level for flood possibility
const int THRESHOLD_RAIN_VALUE = 2000;    // Define a threshold rain value for flood possibility
const float THRESHOLD_HUMIDITY = 80.0;   // Define a threshold humidity value for flood possibility
const int THRESHOLD_HIGH_TEMPERATURE = 30; // Define a threshold high temperature for flood possibility


void setup(){
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  dht.begin();
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
 float water=waterlevel();
 int rain=rainsensor();
 float humidity= getHumidity();
 int temperature=gettemp();
 float posibility=calculateFloodPossibility(water,rain,humidity,temperature);
 
  

 ThingSpeak.writeField(myNumber, 1, water, myWriteAPIKey);
 ThingSpeak.writeField(myNumber, 2, rain, myWriteAPIKey);
 ThingSpeak.writeField(myNumber,3,temperature,myWriteAPIKey);
 ThingSpeak.writeField(myNumber,4,humidity,myWriteAPIKey);
 ThingSpeak.writeField(myNumber,5,posibility,myWriteAPIKey);

 delay(2000);

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
  Serial.print("distance is=   ");
  Serial.print(cm);
  Serial.println(" cm ");
  return cm;
  
  
}
int rainsensor(){
  int sensorvalue=analogRead(rainsensorpin);
  if(sensorvalue<3800){
    Serial.print("it is raining outside rainsensor value is= ");
    Serial.print(sensorvalue);
    Serial.print("\n");
    digitalWrite(2,HIGH);
    delay(1000);
    digitalWrite(2,LOW);
    delay(500);
    
  }
  else{
    Serial.print("no rain is there rainsensor value = ");
    Serial.print(sensorvalue);
    Serial.print("\n");
    digitalWrite(2,LOW);
  }
  return sensorvalue;
}

float getHumidity(){
  float humidity = dht.readHumidity(); // Read humidity
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  return humidity;
}

int gettemp(){
  int temp = dht.readTemperature(); // Read humidity
  Serial.print("temperature: ");
  Serial.print(temp);
  Serial.println(" c");
  return temp;
}

float calculateFloodPossibility(float waterLevel, int rain, float humidity, int temperature) {
    float floodPossibility = 0.0;

   
    if (waterLevel < THRESHOLD_WATER_LEVEL) {
        floodPossibility += 30.0;  
    }

    if (rain < THRESHOLD_RAIN_VALUE) {
        floodPossibility += 20.0;  
    }

    if (humidity > THRESHOLD_HUMIDITY) {
        floodPossibility += 10.0;  
    }

    if (temperature > THRESHOLD_HIGH_TEMPERATURE) {
        floodPossibility += 15.0;  
    }

    
    if (floodPossibility > 100.0) {
        floodPossibility = 100.0;
    }


  Serial.print("flood posibility: ");
  Serial.print(floodPossibility);
  Serial.println(" %");
  Serial.print("\n\n");
  return floodPossibility;
}
