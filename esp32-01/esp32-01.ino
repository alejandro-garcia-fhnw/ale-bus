#include <WiFi.h>
#include <PubSubClient.h>
#include <EasyButton.h>
#include "DHT.h"

#define BUTTON1_PIN 5
EasyButton button1(BUTTON1_PIN);
#define BUTTON2_PIN 4
EasyButton button2(BUTTON2_PIN);
/*
 * ESP32-01 - Hinten
 * 
 * 5 Relays - MQTT Topic "esp32-01/light/bedL"
 * 2 Buttons - MQTT Topic "esp32-01/buttons/bedR"
 * 1 DHT22 - MQTT Topic "esp32-01/temp", "esp32-01/hum"
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
const int RELAY_1 = 32;
const int RELAY_2 = 33;
const int RELAY_3 = 25;
const int RELAY_4 = 26;
const int RELAY_5 = 27;

int button1_status;
int button2_status;

const int dht22 = 2;
DHT dht(dht22,DHT22);

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
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");

      // ... and resubscribe
      client.subscribe("esp32-01/relay1");
      client.subscribe("esp32-01/relay2");
      client.subscribe("esp32-01/relay3");
      client.subscribe("esp32-01/relay4");
      client.subscribe("esp32-01/relay5");

      client.subscribe("esp32-01/button1");
      client.subscribe("esp32-01/button2");
      
      
      //Once connected, publish an announcement...
      client.publish("esp32-01/status", "connected");
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
    
    if(topicS == "esp32-01/relay1") {
      Serial.println("-------Relay 1-----");
      
      if ((char)payload[0] == '1') {
        digitalWrite(RELAY_1, HIGH);
        Serial.println("on");
      }
      else {
        digitalWrite(RELAY_1, LOW);
        Serial.println("off");
      }
    }
    
    if(topicS == "esp32-01/relay2") {
      Serial.println("-------Relay 2-----");
      
      if ((char)payload[0] == '1') {
        digitalWrite(RELAY_2, HIGH);
        Serial.println("on");
      }
      else {
        digitalWrite(RELAY_2, LOW);
        Serial.println("off");
      }
    }
    
    if(topicS == "esp32-01/relay3") {
      Serial.println("-------Relay 3-----");
      
      if ((char)payload[0] == '1') {
        digitalWrite(RELAY_3, HIGH);
        Serial.println("on");
      }
      else {
        digitalWrite(RELAY_3, LOW);
        Serial.println("off");
      }
    }
    
    if(topicS == "esp32-01/relay4") {
      Serial.println("-------Relay 4-----");
      
      if ((char)payload[0] == '1') {
        digitalWrite(RELAY_4, HIGH);
        Serial.println("on");
      }
      else {
        digitalWrite(RELAY_4, LOW);
        Serial.println("off");
      }
    }
    
    if(topicS == "esp32-01/relay5") {
      Serial.println("-------Relay 5-----");
      
      if ((char)payload[0] == '1') {
        digitalWrite(RELAY_5, HIGH);
        Serial.println("on");
      }
      else {
        digitalWrite(RELAY_5, LOW);
        Serial.println("off");
      }
    }
    
    //Buttons
    
    if(topicS == "esp32-01/button1") {
      if ((char)payload[0] == '1') {
        button1_status = 1;
      }
      else {
        button1_status = 0;
      }
    }
    
    if(topicS == "esp32-01/button2") {   
      if ((char)payload[0] == '1') {
        button2_status = 1;
      }
      else {
        button2_status = 0;
      }
    }
    
    
    //Serial.print("topic:");
    //Serial.println(topic);
    //Serial.print("data:");  
    //Serial.write(payload, length);
    //Serial.println();
}

// Callback function to be called when the button is pressed.
void on1Pressed() {
  if(button1_status == 0) {
    client.publish("esp32-01/button1", "1");
  }
  if(button1_status == 1) {
    client.publish("esp32-01/button1", "0");
  }
  Serial.println("Button 1 pressed" + button1_status);
  delay(500);
}
void on2Pressed() {
  if(button2_status == 0) {
    client.publish("esp32-01/button2", "1");
  }
  if(button2_status == 1) {
    client.publish("esp32-01/button2", "0");
  }
  Serial.println("Button 2 pressed" + button2_status);
  delay(500);
}


void setup() {
  Serial.begin(115200);

  dht.begin();
  
  // Initialize buttons.
  button1.begin();
  button2.begin();
  
  // Add the callback function to be called when the button is pressed.
  button1.onPressed(on1Pressed);
  button2.onPressed(on2Pressed);

  pinMode(RELAY_1,OUTPUT);
  pinMode(RELAY_2,OUTPUT);
  pinMode(RELAY_3,OUTPUT);
  pinMode(RELAY_4,OUTPUT);
  pinMode(RELAY_5,OUTPUT);
    
  Serial.setTimeout(500);// Set time out for 
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  reconnect();
}


void loop() {
   client.loop();
   
    button1.read();
    button2.read();

    long now = millis();
    // If last time is longer than xx Seconds
    if (now - lastMsg > 5000) {
      lastMsg = now;

      //Read Temp + Hum
      float tempDHT22 = dht.readTemperature();
      float humDHT22 = dht.readHumidity();
      
      // Convert the Temperature to a char array and publish it
      char temp1String[8];
      dtostrf(tempDHT22, 1, 2, temp1String);
      Serial.print("Temperature: ");
      Serial.println(temp1String);
      client.publish("esp-01/temp", temp1String);

      // Convert Humidity value to a char array and publish it
      char temp2String[8];
      dtostrf(humDHT22, 1, 2, temp2String);
      Serial.print("Humidity: ");
      Serial.println(temp2String);
      client.publish("esp-01/hum", temp2String);
    } 
 }
