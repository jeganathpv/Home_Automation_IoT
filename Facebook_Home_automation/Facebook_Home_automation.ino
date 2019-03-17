
#include <ESP8266WiFi.h>

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define D4 2 // GPIO4
#define D8 15 // GPIO8

String  Request;
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME  "jeganathpv"
#define AIO_KEY  "da8e909832fe42c18810dbcaa5fc2204"
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe homefeed = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/homefeed");
boolean MQTT_connect();

boolean MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return true;
  }  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {
    mqtt.disconnect();
    delay(2000);
    retries--;
    if (retries == 0) {
      return false;
    }
  } return true;
}

void setup()
{
  Serial.begin(115200);
  Request = "";

  WiFi.disconnect();
  delay(3000);
  Serial.println("Start IoT");
  WiFi.begin("Jaddu", "2444666668888888");
  while ((!(WiFi.status() == WL_CONNECTED))) {
    delay(300);
    Serial.print(".");

  }
  Serial.println("Wifi Connected");
  Serial.println("Your IP is");
  Serial.println((WiFi.localIP().toString()));

  mqtt.subscribe(&homefeed);

  pinMode(D4, OUTPUT);
  pinMode(D8, OUTPUT);

  digitalWrite(D4, LOW);
  digitalWrite(D8, LOW);
}


void loop()
{

  if (MQTT_connect()) {
    Adafruit_MQTT_Subscribe *subscription_name;
    while ((subscription_name = mqtt.readSubscription(5000))) {
      if (subscription_name == &homefeed) {
        Request = ((char *)homefeed.lastread);
        if (Request == "d4on") {
          digitalWrite(D4, HIGH);
          Serial.println("light turn ON");
        }
        if (Request == "d4off") {
          digitalWrite(D4, LOW);
          Serial.println("Red light turn OFF");
        }

        if (Request == "d8on") {
          digitalWrite(D8, HIGH);
          Serial.println("Fan turn ON");
        }
        if (Request == "d8off") {
          digitalWrite(D8, LOW);
          Serial.println("Fan turn OFF");
        }
      }
    }
  }
}
