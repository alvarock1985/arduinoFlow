#include <Ethernet.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include "RestClient.h"


#define delayTime 300  // Time in seconds beetwen sendings
#define IP "170.239.86.239" // Server IP
#define PORT 8080     // Server Port
volatile int NbTopsFan; //measiring the rising edges of the signal
int Calc;
int hallSensor =2; //pint loc of the sensor
int cubeMeter;
RestClient client = RestClient(IP, PORT);

void rpm() {

  //measures the rising and falling edgesof the hall effect
  NbTopsFan++; 
  
  };
  
void setup() {

  pinMode(hallSensor, INPUT);
  // put your setup code here, to run once:
    Serial.begin(9600);
    attachInterrupt(0, rpm, RISING);
  // Connect via DHCP
  Serial.println("connect to network");
  client.dhcp();
  Serial.println("Setup!");
}

String response;
void loop() {
  // put your main code here, to run repeatedly:
  NbTopsFan = 0;
  sei();
  delay(600000);
  cli();
  Calc = (NbTopsFan * 60 / 7.5);
  if(Calc<0){
    Calc = (Calc * -1);
  }else{
    Calc = Calc;
  }
  cubeMeter = (Calc/1000);
  response = "";
  StaticJsonBuffer<200> jsonBuffer;
  char json[256];
  JsonObject& root = jsonBuffer.createObject();
  root["data"] = cubeMeter;
  root["sensorId"] = 54;
  root.printTo(json, sizeof(json));
  Serial.println(json);
  int statusCode = client.post("/EmuSensor/webapi/datasensors/add/proto", json, &response);
  Serial.print("Status code from server: ");
  Serial.println(statusCode);
  Serial.print("Response body from server: ");
  Serial.println(response);
 
}
