#include <WiFi.h>
#include <PubSubClient.h>
#include <EasyButton.h>
#include "DHT.h"

#define BUTTON1_PIN 5
EasyButton button1(BUTTON1_PIN);
#define BUTTON2_PIN 4
EasyButton button2(BUTTON2_PIN);
#define BUTTON3_PIN 18
EasyButton button3(BUTTON3_PIN);
#define BUTTON4_PIN 19
EasyButton button4(BUTTON4_PIN);
/*
 * ESP32-01 - Hinten
 * 
 * 6 Relays
 *    MQTT Topic: "esp32-01/relay1,2,3,4,5"
 *    GPIO PINS: 32,33,25,26,27,14
 * 
 * 4 Buttons
 *    MQTT Topic: "esp32-01/button1,2"
 *    GPIO Pins: 5,4,18,19
 * 
 * 1 DHT22
 *    MQTT Topic: "esp32-01/temp", "esp32-01/hum"
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
const int RELAY_6 = 14;

int button1_status;
int button2_status;
int button3_status;
int button4_status;

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
    // Create a client ID
    String clientId = "esp32-01";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(),"esp32-01/status",0,1,"offline")) {
      Serial.println("connected");

      // ... and resubscribe
      client.subscribe("esp32-01/relay1");
      client.subscribe("esp32-01/relay2");
      client.subscribe("esp32-01/relay3");
      client.subscribe("esp32-01/relay4");
      client.subscribe("esp32-01/relay5");
      client.subscribe("esp32-01/relay6");

      client.subscribe("esp32-01/button1");
      client.subscribe("esp32-01/button2");
      client.subscribe("esp32-01/button3");
      client.subscribe("esp32-01/button4");
      delay(1000);
      //Once connected, publish an announcement...
      client.publish("esp32-01/status", "online", true);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
    }
  }
}

void callback(char* topic, byte *payload, unsigned int length) {
    String topicS(topic);
    
    //alive message
    //client.publish("esp32-01/status", "connected");
    
    if(topicS == "esp32-01/relay1") {
      Serial.println("-------Relay 1-----");
      
      if ((char)payload[0] == '1') {
        digitalWrite(RELAY_1, LOW);
        Serial.println("on");
      }
      else {
        digitalWrite(RELAY_1, HIGH);
        Serial.println("off");
      }
    }
    
    if(topicS == "esp32-01/relay2") {
      Serial.println("-------Relay 2-----");
      
      if ((char)payload[0] == '1') {
        digitalWrite(RELAY_2, LOW);
        Serial.println("on");
      }
      else {
        digitalWrite(RELAY_2, HIGH);
        Serial.println("off");
      }
    }
    
    if(topicS == "esp32-01/relay3") {
      Serial.println("-------Relay 3-----");
      
      if ((char)payload[0] == '1') {
        digitalWrite(RELAY_3, LOW);
        Serial.println("on");
      }
      else {
        digitalWrite(RELAY_3, HIGH);
        Serial.println("off");
      }
    }
    
    if(topicS == "esp32-01/relay4") {
      Serial.println("-------Relay 4-----");
      
      if ((char)payload[0] == '1') {
        digitalWrite(RELAY_4, LOW);
        Serial.println("on");
      }
      else {
        digitalWrite(RELAY_4, HIGH);
        Serial.println("off");
      }
    }
    
    if(topicS == "esp32-01/relay5") {
      Serial.println("-------Relay 5-----");
      
      if ((char)payload[0] == '1') {
        digitalWrite(RELAY_5, LOW);
        Serial.println("on");
      }
      else {
        digitalWrite(RELAY_5, HIGH);
        Serial.println("off");
      }
    }

    if(topicS == "esp32-01/relay6") {
      Serial.println("-------Relay 6-----");
      
      if ((char)payload[0] == '1') {
        digitalWrite(RELAY_6, LOW);
        Serial.println("on");
      }
      else {
        digitalWrite(RELAY_6, HIGH);
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

    if(topicS == "esp32-01/button3") {   
      if ((char)payload[0] == '1') {
        button3_status = 1;
      }
      else {
        button3_status = 0;
      }
    }

    if(topicS == "esp32-01/button4") {   
      if ((char)payload[0] == '1') {
        button4_status = 1;
      }
      else {
        button4_status = 0;
      }
    }
    
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
void on3Pressed() {
  if(button3_status == 0) {
    client.publish("esp32-01/button3", "1");
  }
  if(button3_status == 1) {
    client.publish("esp32-03/button3", "0");
  }
  Serial.println("Button 3 pressed" + button3_status);
  delay(500);
}
void on4Pressed() {
  if(button4_status == 0) {
    client.publish("esp32-04/button4", "1");
  }
  if(button4_status == 1) {
    client.publish("esp32-04/button4", "0");
  }
  Serial.println("Button 4 pressed" + button4_status);
  delay(500);
}


void setup() {
  Serial.begin(115200);

  dht.begin();
  
  // Initialize buttons.
  button1.begin();
  button2.begin();
  button3.begin();
  button4.begin();
  
  // Add the callback function to be called when the button is pressed.
  button1.onPressed(on1Pressed);
  button2.onPressed(on2Pressed);
  button3.onPressed(on3Pressed);
  button4.onPressed(on4Pressed);

  pinMode(RELAY_1,OUTPUT);
  pinMode(RELAY_2,OUTPUT);
  pinMode(RELAY_3,OUTPUT);
  pinMode(RELAY_4,OUTPUT);
  pinMode(RELAY_5,OUTPUT);
  pinMode(RELAY_6,OUTPUT);
    
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
    button3.read();
    button4.read();

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
      client.publish("esp32-01/temp", temp1String);

      // Convert Humidity value to a char array and publish it
      char temp2String[8];
      dtostrf(humDHT22, 1, 2, temp2String);
      Serial.print("Humidity: ");
      Serial.println(temp2String);
      client.publish("esp32-01/hum", temp2String);
    } 
 }
