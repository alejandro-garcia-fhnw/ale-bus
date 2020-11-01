#include <WiFi.h>
#include <PubSubClient.h>
#include <EasyButton.h>
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define BUTTON3_PIN 25
EasyButton button3(BUTTON3_PIN);
#define BUTTON4_PIN 26
EasyButton button4(BUTTON4_PIN);
#define BUTTON5_PIN 27
EasyButton button5(BUTTON5_PIN);
/*
 * ESP32-02 - Küche
 * 
 * 3 Buttons
 *    MQTT Topic: "esp32-02/button3,4,5"
 *    GPIO PINS:  25, 26, 27
 * 
 * 1 DHT22
 *    MQTT Topic: "esp32-02/temp", "esp32-02/hum"
 *    GPIO PIN:   2
 * 
 * 1 DS18B20
 *    MQTT Topic: "esp32-02/temp-outside"
 *    GPIO PIN:   4
 * 
 * 
 * IP: xxx.xxx.x.xxx
 * 
 */
 
// Update these with values suitable for your network.
const char* ssid = "ale-bus";
const char* password = "sitzebimpisse";
const char* mqtt_server = "192.168.8.112";

WiFiClient wifiClient;
PubSubClient client(wifiClient);
long lastMsg = 0;
char msg[50];
int value = 0;

// GPIOs

int button3_status;
int button4_status;
int button5_status;

const int dht22 = 2;
DHT dht(dht22,DHT22);

const int ds18b20 = 4;
OneWire oneWire(ds18b20);
DallasTemperature sensorOne(&oneWire);

void setup_wifi() {   
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    randomSeed(micros());
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "esp32-02";
    // Attempt to connect
    if (client.connect(clientId.c_str(),"esp32-02/status",0,1,"offline")) {
      Serial.println("connected");

      // ... and resubscribe

      client.subscribe("esp32-02/button3");
      client.subscribe("esp32-02/button4");
      client.subscribe("esp32-03/button5");
      
      
      //Once connected, publish an announcement...
      client.publish("esp32-02/status", "online", true);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte *payload, unsigned int length) {
    String topicS(topic);
    
    //Buttons
    
    if(topicS == "esp32-02/button3") {
      if ((char)payload[0] == '1') {
        button3_status = 1;
      }
      else {
        button3_status = 0;
      }
    }
    
    if(topicS == "esp32-02/button4") {   
      if ((char)payload[0] == '1') {
        button4_status = 1;
      }
      else {
        button4_status = 0;
      }
    }

    if(topicS == "esp32-02/button5") {   
      if ((char)payload[0] == '1') {
        button5_status = 1;
      }
      else {
        button5_status = 0;
      }
    }

}

// Callback function to be called when the button is pressed.
void on3Pressed() {
  if(button3_status == 0) {
    client.publish("esp32-02/button3", "1");
  }
  if(button3_status == 1) {
    client.publish("esp32-02/button3", "0");
  }
  Serial.println("Button 3 pressed" + button3_status);
  delay(500);
}
void on4Pressed() {
  if(button4_status == 0) {
    client.publish("esp32-02/button4", "1");
  }
  if(button4_status == 1) {
    client.publish("esp32-02/button4", "0");
  }
  Serial.println("Button 4 pressed" + button4_status);
  delay(500);
}
void on5Pressed() {
  if(button5_status == 0) {
    client.publish("esp32-02/button5", "1");
  }
  if(button5_status == 1) {
    client.publish("esp32-02/button5", "0");
  }
  Serial.println("Button 5 pressed" + button5_status);
  delay(500);
}


void setup() {
  Serial.begin(115200);

  dht.begin();

  sensorOne.begin();
  
  // Initialize buttons.
  button3.begin();
  button4.begin();
  button5.begin();
  
  // Add the callback function to be called when the button is pressed.
  button3.onPressed(on3Pressed);
  button4.onPressed(on4Pressed);
  button5.onPressed(on5Pressed);

    
  Serial.setTimeout(500);// Set time out for 
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  reconnect();
}


void loop() {
   client.loop();
   

    button3.read();
    button4.read();
    button5.read();

    long now = millis();
    // If last time is longer than xx Seconds
    if (now - lastMsg > 5000) {
      lastMsg = now;

      //Read Temp + Hum
      float tempDHT22 = dht.readTemperature();
      float humDHT22 = dht.readHumidity();

      sensorOne.requestTemperatures();
      float tempOutisde = sensorOne.getTempCByIndex(0);
      
      // Convert the Temperature to a char array and publish it
      char temp1String[8];
      dtostrf(tempDHT22, 1, 2, temp1String);
      Serial.print("Temperature: ");
      Serial.println(temp1String);
      client.publish("esp32-02/temp", temp1String);

      // Convert Humidity value to a char array and publish it
      char temp2String[8];
      dtostrf(humDHT22, 1, 2, temp2String);
      Serial.print("Humidity: ");
      Serial.println(temp2String);
      client.publish("esp32-02/hum", temp2String);

      // Convert the Temperature to a char array and publish it
      char temp3String[8];
      dtostrf(tempOutisde, 1, 2, temp3String);
      Serial.print("Temperature Outside: ");
      Serial.println(temp3String);
      client.publish("esp32-02/temp-outside", temp3String);
    } 
 }
