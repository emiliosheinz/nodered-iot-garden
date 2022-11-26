#include <PubSubClient.h>
#include <WiFi.h>
#include <DHTesp.h>
#include <cstring>

#define PUBLISH_DELAY 2000
#define TOPIC_PUBLISH_PARAMS "iot-garden-parameters"
#define TOPIC_PUBLISH_GET_CONFIG "get-iot-garden-global-config"
#define TOPIC_SUBSCRIBE_CONFIG "iot-garden-global-config"
#define MQTT_ID "esp32-franz-mqtt"
#define BROKER_PORT 1883
#define DHT_PIN 21
#define PIN_LED 18

const char *SSID = "Wokwi-GUEST";
const char *PASSWORD = "";
const char *BROKER_MQTT = "broker.hivemq.com";
static char message[20] = {};

unsigned long publishUpdate;
int minHumidity, maxHumidity;
bool hasConfig = false;

WiFiClient espClient;
PubSubClient MQTT(espClient);
DHTesp dhtSensor;

void initWiFi(void);
void initMQTT(void);
void callbackMQTT(char *topic, byte *payload, unsigned int length);
void reconnectWiFi(void);
void reconnectMQTT(void);
void checkWiFiAndMQTT(void);
float getHumidity(void);
float getTemperature(void);

void initWiFi(void) {
  delay(10);
  Serial.print("[WIFI] Conectando-se na rede ");
  Serial.println(SSID);
  reconnectWiFi();
}

void initMQTT(void) {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(callbackMQTT);
}

void callbackMQTT(char *topic, byte *payload, unsigned int length) {
  char msg[length + 1];
  memcpy(msg, payload, length);
  msg[length] = '\0'; 

  char * token = strtok(msg, " ");
   if (strcmp("MIN_HUMIDITY", token) == 0) {
      minHumidity = atoi(strtok(NULL, " "));
   } else {
      maxHumidity = atoi(strtok(NULL, " "));
   }
}

void reconnectWiFi(void) {
  if (WiFi.status() == WL_CONNECTED) return;
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("[WiFi] Conectado a rede com sucesso!");
  Serial.print("[WiFi] IP obtido: ");
  Serial.println(WiFi.localIP());
}

void reconnectMQTT(void) {
  while (!MQTT.connected()) {
    Serial.print("[MQTT] Conectando-se ao broker MQTT ");
    Serial.println(BROKER_MQTT);
    if (MQTT.connect(MQTT_ID)) {
      Serial.println("[MQTT] Conectado ao broker com sucesso!");
       MQTT.subscribe(TOPIC_SUBSCRIBE_CONFIG);
    } else {
      Serial.println("[MQTT] Falha em conectar ao broker!");
      delay(1500);
    }
  }
}

void checkWiFiAndMQTT(void) {
  if(!MQTT.connected()) reconnectMQTT();
  reconnectWiFi();
}

float getHumidity(void) {
  TempAndHumidity data = dhtSensor.getTempAndHumidity();

  if (!(isnan(data.humidity)))
    return data.humidity;
  else
    return -99.99;
}

float getTemperature(void) {
  TempAndHumidity data = dhtSensor.getTempAndHumidity();

  if (!(isnan(data.temperature)))
    return data.temperature;
  else
    return -99.99;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  initWiFi();
  initMQTT();
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  if ((millis() - publishUpdate) >= PUBLISH_DELAY) {
    publishUpdate = millis();
    checkWiFiAndMQTT();
    float humidity = getHumidity();
    float temperature = getTemperature();
    sprintf(message, "SOIL_HUMIDITY %.2f,AIR_HUMIDITY %.2f,AIR_TEMPERATURE %.2f", humidity, humidity, temperature);
    Serial.println(message);
    MQTT.publish(TOPIC_PUBLISH_PARAMS, message);
    if (minHumidity > 0 && maxHumidity > 0) {
      if (humidity < minHumidity) {
        digitalWrite(PIN_LED, HIGH);
        Serial.println("LED ligado por comando MQTT");
      } else {
        digitalWrite(PIN_LED, LOW);
      }
    } else {
      MQTT.publish(TOPIC_PUBLISH_GET_CONFIG, "MIN_HUMIDITY");
      MQTT.publish(TOPIC_PUBLISH_GET_CONFIG, "MAX_HUMIDITY");
    }
    
    // exibir umidade no lcd
    MQTT.loop();
  }
}
