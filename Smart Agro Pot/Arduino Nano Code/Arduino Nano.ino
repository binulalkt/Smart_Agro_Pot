//#include
#include <EEPROM.h>//tds
#include "GravityTDS.h"//tds
#include <BH1750.h>//sunlight
#include <Wire.h>//sunlight
#include <Adafruit_Sensor.h>//temp
#include <DHT.h>//temp
#include <DHT_U.h>//temp
#include "DFRobot_PH.h"//pH

//#define
#define TdsSensorPin A1//tds
#define DHTPIN 2   //temp
#define DHTTYPE DHT11 //temp   
#define sensorPower 7
#define sensorPin A3
#define PH_PIN A0

//function call
GravityTDS gravityTds; //tds
BH1750 lightMeter; //sunlight
DHT_Unified dht(DHTPIN, DHTTYPE);

DFRobot_PH ph;
//intialise
float tdsValue = 0;
float voltage,phValue,temperature = 28;
int val = 0;
void setup() 
{
  Serial.begin(9600);

  //begin
  dht.begin();
  //tds begin
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);  
  gravityTds.setAdcRange(1024); 
  gravityTds.begin(); 
  //sunlight begin
  Wire.begin(); 
  lightMeter.begin();
  //waterlevel
  pinMode(sensorPower, OUTPUT);
  digitalWrite(sensorPower, LOW);
  //ph
  ph.begin();
  
}

void loop() {
  //tds sensor
  gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
  gravityTds.update();  //sample and calculate 
  tdsValue = gravityTds.getTdsValue();  // then get the value
  Serial.print(tdsValue,0);
  Serial.print(",");
  delay(1000);
 
  //DHT sensor
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.print("31");
  }
  else {
    Serial.print(event.temperature);
    Serial.print(",");
  }
  
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.print("68");
  }
  else {
    Serial.print(event.relative_humidity);
    Serial.print(",");
  }
  //sunlight
  float lux = lightMeter.readLightLevel();
  Serial.print(lux);
  Serial.print(",");
  delay(1000);
  //waterlevel
  int level = readSensor();
  Serial.print(level);
  Serial.print(",");
  delay(1000);
  //pH
  static unsigned long timepoint = millis();
    if(millis()-timepoint>1000U){                  //time interval: 1s
        timepoint = millis();       // read your temperature sensor to execute temperature compensation
        voltage = analogRead(PH_PIN)/1024.0*5000;  // read the voltage
        phValue = ph.readPH(voltage,temperature);  // convert voltage to pH with temperature compensation
        phValue=phValue-(event.relative_humidity*0.01);
        Serial.print(phValue,2);
    }
    ph.calibration(voltage,temperature); 
           
}
