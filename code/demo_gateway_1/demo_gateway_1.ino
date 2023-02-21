/* demo_gateway_1.ino
 * Pair with demo_node_1.ino
 * Last edited: 2023-02-21
 *
 * By: Luthfi Pratama
 */
 
//Importing libraries
//LORA libraries
#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <SPI.h>
//MQTT libraries
#include <WiFi.h>
#include <PubSubClient.h>
//secret library
#include "secrets.h"

//LORA settings
#define RFM95_CS 15
#define RFM95_RST 4
#define RFM95_INT 2

#define GATEWAY_ADDRESS 10
#define RTU_ADDRESS 11

//Creating objects
// Singleton instance of the radio driver
RH_RF95 driver(RFM95_CS, RFM95_INT);
// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, GATEWAY_ADDRESS);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

//Network credentials
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;

//Broker parameters
char *mqttServer = MQTT_SERVER;
int mqttPort = MQTT_PORT;

String id = DEVICE_ID;  // Name of our device, must be unique
String topic_publish = String(TOPIC_BASE) + "pubs/" + String(DEVICE_ID);  // Topic to publish to
String topic_subscribe = String(TOPIC_BASE) + "subs/" + String(DEVICE_ID);  // Topic to subcribe to

String dataset;

void setupMQTT(){
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(callback);
}

void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
    Serial.println("Reconnecting to MQTT Broker...");
    
    if (mqttClient.connect(id.c_str(), MQTT_USERNAME, MQTT_PASS)) {
      Serial.println("Connected.");
      mqttClient.subscribe(topic_subscribe.c_str());     
    }      
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Callback - ");
  Serial.print("Message:");
  String command;

  for (int i =0; i < length; i++){
    command += String((char)message[i]);
  }
  
  Serial.println(command);
  Serial.println(sizeof(command));
  delay(1000);
}

void setup() {
  //Serial setup
  Serial.begin(115200);
  while (!Serial) ; // Wait for serial port to be available

  //LORA setup
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
  
  if (!manager.init())
    Serial.println("init failed");

  driver.setFrequency(440.0);
  driver.setTxPower(18, false);

  //MQTT setup
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");

  setupMQTT();
}

uint8_t data[] = "REQ_RTU13";
// Dont put this on the stack:
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];

void loop() {
  if (!mqttClient.connected())
  {
    reconnect();
  }
  mqttClient.loop();
  
  Serial.println("Sending to rf95_reliable_datagram_server");
    
  // Send a message to manager_server
  if (manager.sendtoWait(data, sizeof(data), RTU_ADDRESS))
  {
    // Now wait for a reply from the server
    uint8_t len = sizeof(buf);
    uint8_t from;   
    if (manager.recvfromAckTimeout(buf, &len, 2000, &from))
    {
      Serial.print("got reply from : 0x");
      Serial.println(from, HEX);
      dataset = (char *) buf;
      
      Serial.println(dataset);
      mqttClient.publish(topic_publish.c_str(), dataset.c_str());
    }
    else
    {
      Serial.println("No reply, is rf95_reliable_datagram_server running?");
    }
  }
  else
    Serial.println("sendtoWait failed");
  delay(500);

}
