#include <Arduino.h>
#include <Servo.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include "analytics.h"
#include "config.h"
constexpr uint8_t MIC_PIN=A0,SERVO_PIN=9,RELAY_PIN=5;
Servo vent; WiFiClient network; PubSubClient mqtt(network); Usage usage;
uint32_t last=0,lastConnect=0,lastSound=0;bool heard=false,requested=false;
void command(char*,byte* p,unsigned int n){
 if(n==2&&p[0]=='o'&&p[1]=='n')requested=true;
 else if(n==3&&p[0]=='o'&&p[1]=='f'&&p[2]=='f')requested=false;
}
void setup(){
 pinMode(RELAY_PIN,OUTPUT);digitalWrite(RELAY_PIN,LOW);
 vent.attach(SERVO_PIN);vent.write(0);analogReadResolution(10);Serial.begin(115200);
 mqtt.setServer(MQTT_HOST,MQTT_PORT);mqtt.setCallback(command);mqtt.setSocketTimeout(1);
 if(WIFI_SSID[0])WiFi.begin(WIFI_SSID,WIFI_PASSWORD);last=millis();
}
void loop(){
 uint32_t now=millis();
 if(WiFi.status()==WL_CONNECTED&&MQTT_HOST[0]){
  if(!mqtt.connected()&&now-lastConnect>=10000){lastConnect=now;if(mqtt.connect("omp-007"))mqtt.subscribe("omp/007/command");}
  mqtt.loop();
 }
 if(now-last<1000)return;
 uint32_t dt=now-last;last=now;
 int lo=1023,hi=0;
 for(int i=0;i<128;++i){int x=analogRead(MIC_PIN);lo=min(lo,x);hi=max(hi,x);delayMicroseconds(100);}
 if(hi-lo>=SOUND_THRESHOLD){lastSound=now;heard=true;}
 bool active=requested||(heard&&now-lastSound<5000);
 usage.advance(dt,active);
 digitalWrite(RELAY_PIN,active?HIGH:LOW);vent.write(active?90:0);
 char out[240];
 snprintf(out,sizeof(out),"{\"project_id\":7,\"day\":%lu,\"week\":%lu,\"daily_ms\":%llu,\"weekly_ms\":%llu,\"events\":%lu,\"sound_peak_to_peak\":%d,\"active\":%s}",(unsigned long)(usage.uptimeMs/86400000ULL),(unsigned long)(usage.uptimeMs/604800000ULL),(unsigned long long)usage.dailyMs,(unsigned long long)usage.weeklyMs,(unsigned long)usage.events,hi-lo,active?"true":"false");
 Serial.println(out);if(mqtt.connected())mqtt.publish("omp/007/telemetry",out);
}
