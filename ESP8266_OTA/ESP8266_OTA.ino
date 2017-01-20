/** Для возможности прошивки по сетевому порту,
 *  необходимо установить последнюю версию python 
 *  Скачать по ссылке: https://www.python.org/downloads/
**/
#include <ESP8266WiFi.h> //Библиотека для работы с WIFI 
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h> // Библиотека для OTA-прошивки

const char* ssid = "Musa Pro"; //Имя точки доступа WIFI 
const char* password = "Gigabyte2014"; //пароль точки доступа WIFI 

void setup() {
//  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(5000);
    ESP.restart();
  }
  ArduinoOTA.setHostname("ESP8266-00001"); // Задаем имя сетевого порта
  //ArduinoOTA.setPassword((const char *)"0000"); // Задаем пароль доступа для удаленной прошивки
  ArduinoOTA.begin(); // Инициализируем OTA
  
  pinMode(14,OUTPUT);
  pinMode(2,OUTPUT);
  }

void loop() {
ArduinoOTA.handle(); // Всегда готовы к прошивке
digitalWrite(14,1);
delay(100);
digitalWrite(14,0);
delay(100);  


digitalWrite(2,1);
delay(100);
digitalWrite(2,0);
delay(100); 
}
