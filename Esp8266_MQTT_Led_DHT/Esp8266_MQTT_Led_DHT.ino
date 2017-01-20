// реле  подлкючен к 5  пину
// Датчик температуры DHT к 2 пину

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <DHT.h>
#include <ArduinoOTA.h>

DHT dht(2, DHT11); //определил пин датчика температуры
#define ONE_WIRE_BUS 5 // Определение пина Ds18b20 на 
#define BUFFER_SIZE 100 // размер буфера
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempDS(&oneWire);

  long previousMillis = 0;
  long interval = 10000;
  const char *ssid = "TP-LINK_C2FF16"; // Имя вайфай точки доступа
  const char *pass = "intermetso"; // Пароль от точки доступа
  const char *mqtt_server = "m20.cloudmqtt.com"; // Имя сервера MQTT
  const int mqtt_port =   13233; // Порт для подключения к серверу MQTT
  const char *mqtt_user = "dczizsfj"; // Логи от сервер
  const char *mqtt_pass = "4uSxu7dHYdV5"; // Пароль от сервера
  bool LedState = false; // Функция получения данных от сервера
  void callback(const MQTT::Publish& pub)
{
  Serial.print(pub.topic());   // выводим в сериал порт название топика
  Serial.print(" => ");
  Serial.print(pub.payload_string()); // выводим в сериал порт значение полученных данных
  
  String payload = pub.payload_string();
  
  if(String(pub.topic()) == "test/relay1") // проверяем из нужного ли нам топика пришли данные ( в приложении указываешь этот топик и сможешь управлять реле)
  {
  int stled = payload.toInt(); // преобразуем полученные данные в тип integer
  digitalWrite(16,stled);  //  включаем или выключаем светодиод(в моем случае реле, а после лампа) в зависимоти от полученных значений данных
  }
}

WiFiClient wclient;      
PubSubClient client(wclient, mqtt_server, mqtt_port);

void setup() {
  dht.begin();      //запускаем датчик DHT11
  tempDS.begin();     //запускаем датчик DS18B20
  Serial.begin(115200); 
  delay(10);
  Serial.println();
  Serial.println();                                
  pinMode(16, OUTPUT); //определяем пин на управление реле
    ArduinoOTA.setHostname("ESP8266-00001"); // Задаем имя сетевого порта
  //ArduinoOTA.setPassword((const char *)"0000"); // Задаем пароль доступа для удаленной прошивки
  ArduinoOTA.begin(); // Инициализируем OTA
}

void loop() {
  // подключаемся к wi-fi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    Serial.println("WiFi connected");
    ArduinoOTA.handle();
  }

  // подключаемся к MQTT серверу
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      Serial.println("Connecting to MQTT server");
      if (client.connect(MQTT::Connect("arduinoClient2")
                         .set_auth(mqtt_user, mqtt_pass))) {
        Serial.println("Connected to MQTT server");
        client.set_callback(callback);
        client.subscribe("test/relay1"); // подписывааемся под топик с данными для светодиода
      } else {
        Serial.println("Could not connect to MQTT server");   
      }
    }

    if (client.connected()){
      client.loop();
       unsigned long currentMillis = millis();
    if(currentMillis - previousMillis > interval) {
      float h = dht.readHumidity();
      float f = dht.readTemperature(true);
      float c = dht.readTemperature(false);
       tempDS.requestTemperatures();
      float ds = tempDS.getTempCByIndex(0);
        if (isnan(h) || isnan(f) || isnan(c)) {
          return;
        }
       

      previousMillis = currentMillis;
      client.publish("test/tempF",String(f)); // топик фаренгейта
      client.publish("test/hum",String(h)); // топик влажности
      client.publish("test/tempC",String(c)); // топик температуры цельсия
      client.publish("test/tempDS",String(ds));
  }
  } 
}

}
 // в обещ подключаешь датчик DHT к 2 пину и на сайте mqttcloud.com будешь видить данные автоматически, на них же подписываешься в приложении на андроиде.
 // код датчика сокращен до миннимума в целях чистой работы 100 % рабочий.

 


