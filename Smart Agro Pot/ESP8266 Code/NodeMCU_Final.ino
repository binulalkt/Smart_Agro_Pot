#include <ESP8266WiFi.h>
#include <SimpleTimer.h>
#include <BlynkSimpleEsp8266.h>
#define BLYNK_TEMPLATE_ID "TMPL38XWxuhge"
#define BLYNK_TEMPLATE_NAME "Smart Agro Pot"
char auth[] = "HRtxysz627M43QxZY0giXy1NN57-Hy5R";
char ssid[] = "SSID";
char pass[] = "password";
const int relayPin = D8;
const int switchPin = V6;


SimpleTimer timer;

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(5000L, sendSensorData);  
}

void loop()
{
  Blynk.run();
  timer.run();
}

void sendSensorData(){

  if (Serial.available() > 0) {
    String receivedData = Serial.readStringUntil('\n');
    int commaIndex = receivedData.indexOf(',');

    int tds = Serial.parseInt();       // Read TDS
    Serial.read();
    int temp = Serial.parseInt();      // Read temperature
    Serial.read();
    int humid = Serial.parseInt();     // Read humidity
    Serial.read();
    int sunlight = Serial.parseInt();  // Read sunlight
    Serial.read();
    int waterlevel = Serial.parseInt();  // Read water level
    Serial.read();
    int pH = Serial.parseInt();


  
  int analogValue = analogRead(A0);
  int TDS=analogValue;
  float Temp= 30+(TDS*0.01);
  int Humid= 80+(TDS*0.01);
  Blynk.virtualWrite(V0,tds);
  Blynk.virtualWrite(V1,temp);
  Blynk.virtualWrite(V2,humid);
  Blynk.virtualWrite(V3,sunlight);
  Blynk.virtualWrite(V4,waterlevel);
  Blynk.virtualWrite(V5,pH);

  if ( temp >50) {
    Blynk.logEvent("high_temp", String("High TemperatureDetected!: ") + temp);
    }

  if ( tds < 600 || tds >1300) {
    Blynk.logEvent("abnormal_tds", String("Abnormal TDS : ") + tds);
    }
  if ( waterlevel <300) {
    Blynk.logEvent("low_water_level", String("Low water level : ") + waterlevel);
    }
    
}
}
BLYNK_WRITE(switchPin) {
  int switchStatus = param.asInt();
  
  if (switchStatus == HIGH) {
    digitalWrite(relayPin, LOW);
    Serial.println("LED bulb turned ON");
  } else {
    digitalWrite(relayPin, HIGH);
    Serial.println("LED bulb turned OFF");
  }
}
