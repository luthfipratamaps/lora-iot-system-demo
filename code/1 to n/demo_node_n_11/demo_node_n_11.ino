/* demo_node_n_11.ino
 * Pair with demo_gateway_n.ino
 * Last edited: 2023-02-25
 * 
 * Changelogs:
 * 1.  2023-02-25
 *   - init of this version
 *
 * By: Luthfi Pratama
 */

//Importing libraries
//LORA libraries
#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <SPI.h>

// LORA settings
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2

#define RTU_ADDRESS 11
#define GATEWAY_ADDRESS 10

//Creating objects and instances
//Singleton instance of the radio driver
RH_RF95 driver(RFM95_CS, RFM95_INT);
//Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, RTU_ADDRESS);

// Radio comm variables
uint8_t data[128];
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
String Gateway_Command = String(RTU_ADDRESS);

// Misc
float sensor1, sensor2, sensor3, sensor4;
String dataset;

void setup() {  
  //Serial setup
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  
  //LORA setup
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  
  // manual reset
  digitalWrite(RFM95_RST, LOW); delay(10);
  digitalWrite(RFM95_RST, HIGH); delay(10);
  
  if (!manager.init()) Serial.println("init failed");

  driver.setFrequency(440.0);
  driver.setTxPower(18, false);

  Serial.println("Node" + String(RTU_ADDRESS) + " Started...");
}

void loop() {
  // Read sensor here
  sensor1 = 32.23;
  sensor2 = 76.89;
  sensor3 = 80.12;
  sensor4 = 112.74;

  if (manager.available()){
    // Wait for a message addressed to this node
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (manager.recvfromAck(buf, &len, &from)){
      Serial.print("got request from gateway id: 0x");
      Serial.println(from, HEX);
      Serial.print("request: ");
      Serial.println((char*)buf);

      if(Gateway_Command == (char*)buf){
        Serial.println("Sending response");

        dataset = String(RTU_ADDRESS);
        dataset += "," + String(sensor1);
        dataset += "," + String(sensor2);
        dataset += "," + String(sensor3);
        dataset += "," + String(sensor4);
        dataset.toCharArray(data, 128);

        Serial.println(dataset);
        Serial.println("Done...\n");


      // Send a reply back to gateway
      if (!manager.sendtoWait(data, sizeof(data), from)) 
        Serial.println("sendtoWait failed");
      }
    }
  }
}