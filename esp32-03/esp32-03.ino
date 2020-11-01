#include <WiFi.h>
#include <PubSubClient.h>
#include <EasyButton.h>

#define BUTTON6_PIN 32
EasyButton button6(BUTTON6_PIN);
#define BUTTON7_PIN 33
EasyButton button7(BUTTON7_PIN);
#define BUTTON8_PIN 25
EasyButton button8(BUTTON8_PIN);
#define BUTTON9_PIN 26
EasyButton button9(BUTTON9_PIN);
#define BUTTON10_PIN 27
EasyButton button10(BUTTON10_PIN);
#define BUTTON11_PIN 14
EasyButton button11(BUTTON11_PIN);

/*
 * ESP32-03 - Eingang
 * 
 * 6 Buttons
 *    MQTT Topic: "esp32-02/button6,7,8,9,10,11"
 *    GPIO PINS:  32, 33, 25, 26, 27, 14
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

int button6_status;
int button7_status;
int button8_status;
int button9_status;
int button10_status;
int button11_status;


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
    String clientId = "esp32-03";
    // Attempt to connect
    if (client.connect(clientId.c_str(),"esp32-03/status",1,0,"offline")) {
      Serial.println("connected");

      // ... and resubscribe

      client.subscribe("esp32-03/button6");
      client.subscribe("esp32-03/button7");
      client.subscribe("esp32-03/button8");
      client.subscribe("esp32-03/button9");
      client.subscribe("esp32-03/button10");
      client.subscribe("esp32-03/button11");
      
      
      //Once connected, publish an announcement...
      client.publish("esp32-03/status", "online", true);
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
    
    if(topicS == "esp32-03/button6") {
      if ((char)payload[0] == '1') {
        button6_status = 1;
      }
      else {
        button6_status = 0;
      }
    }
    
    if(topicS == "esp32-03/button7") {   
      if ((char)payload[0] == '1') {
        button7_status = 1;
      }
      else {
        button7_status = 0;
      }
    }

    if(topicS == "esp32-03/button8") {   
      if ((char)payload[0] == '1') {
        button8_status = 1;
      }
      else {
        button8_status = 0;
      }
    }

    if(topicS == "esp32-03/button9") {   
      if ((char)payload[0] == '1') {
        button9_status = 1;
      }
      else {
        button9_status = 0;
      }
    }

    if(topicS == "esp32-03/button10") {   
      if ((char)payload[0] == '1') {
        button10_status = 1;
      }
      else {
        button10_status = 0;
      }
    }

    if(topicS == "esp32-03/button11") {   
      if ((char)payload[0] == '1') {
        button11_status = 1;
      }
      else {
        button11_status = 0;
      }
    }
}

// Callback function to be called when the button is pressed.
void on6Pressed() {
  if(button6_status == 0) {
    client.publish("esp32-03/button6", "1");
  }
  if(button6_status == 1) {
    client.publish("esp32-03/button6", "0");
  }
  Serial.println("Button 6 pressed" + button6_status);
  delay(500);
}
void on7Pressed() {
  if(button7_status == 0) {
    client.publish("esp32-03/button7", "1");
  }
  if(button7_status == 1) {
    client.publish("esp32-03/button7", "0");
  }
  Serial.println("Button 7 pressed" + button7_status);
  delay(500);
}
void on8Pressed() {
  if(button8_status == 0) {
    client.publish("esp32-03/button8", "1");
  }
  if(button8_status == 1) {
    client.publish("esp32-03/button8", "0");
  }
  Serial.println("Button 8 pressed" + button8_status);
  delay(500);
}
void on9Pressed() {
  if(button9_status == 0) {
    client.publish("esp32-03/button9", "1");
  }
  if(button9_status == 1) {
    client.publish("esp32-03/button9", "0");
  }
  Serial.println("Button 9 pressed" + button9_status);
  delay(500);
}
void on10Pressed() {
  if(button10_status == 0) {
    client.publish("esp32-03/button10", "1");
  }
  if(button10_status == 1) {
    client.publish("esp32-03/button10", "0");
  }
  Serial.println("Button 10 pressed" + button10_status);
  delay(500);
}
void on11Pressed() {
  if(button11_status == 0) {
    client.publish("esp32-03/button11", "1");
  }
  if(button11_status == 1) {
    client.publish("esp32-03/button11", "0");
  }
  Serial.println("Button 11 pressed" + button11_status);
  delay(500);
}


void setup() {
  Serial.begin(115200);
  
  // Initialize buttons.
  button6.begin();
  button7.begin();
  button8.begin();
  button9.begin();
  button10.begin();
  button11.begin();
  
  // Add the callback function to be called when the button is pressed.
  button6.onPressed(on6Pressed);
  button7.onPressed(on7Pressed);
  button8.onPressed(on8Pressed);
  button9.onPressed(on9Pressed);
  button10.onPressed(on10Pressed);
  button11.onPressed(on11Pressed);

    
  Serial.setTimeout(500);// Set time out for 
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  reconnect();
}


void loop() {
   client.loop();
   
    button6.read();
    button7.read();
    button8.read();
    button9.read();
    button10.read();
    button11.read();
 }
