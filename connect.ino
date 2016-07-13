#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <PubSubClient.h>
//#include <DHT.h>
//#include "rgb_lcd.h"

//rgb_lcd lcd;
/*
const int colorR = 255;
const int colorG = 0;
const int colorB = 255;
*/
/*/ Variable for Sensors
#define AirPin A0
#define DhtPin D3
float h, t, airValue;
//Variable
DHT dht(DhtPin, DHT11, 15);*/

//varible for wifi
const char* ssid = "ASUS";
const char* password = "1234567890";

//variable for server
const char* server = "192.168.1.237";

//Type command
int cmd;

//variable for UDP
unsigned int localPort = 8888;
unsigned int portUDP = 10001;
boolean connectedUDP = false;
WiFiUDP UDP1;
WiFiUDP UDP2;

boolean udpConnected = false;
char packetBuffer[1]; //buffer to hold incoming packet

//variable for TCP
WiFiClient clientTCP;
unsigned int portTCP = 10002;

//variable for MQTT
unsigned int portMQTT = 1883;
const char* server_mqtt = "test.mosquitto.org";
WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];

//for Random String
static const char alphanum[] =
  "0123456789"
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  "abcdefghijklmnopqrstuvwxyz";  // chars we need for generation

int stringLength = sizeof(alphanum) - 1;

char genRandom(){

    return alphanum[rand() % stringLength];
}

String randomString(int length){
  String str;
  for (int i = 0; i < length; i++){
    str += genRandom();
  }
  return str;
}

void setup() {
  Serial.begin(115200);
  connectWiFi();
  pinMode(D4, OUTPUT);

  connectUDP();

  client.setServer(server, 1883);

/*  dht.begin(); //start sensor
  lcd.begin(16,2);
  lcd.setRGB(colorR, colorG, colorB);*/
  delay(1000);
}

void loop() {
  /*
  //read data Air Sensor
  airValue = analogRead(A0);
  h = dht.readHumidity();
  t = dht.readTemperature();
  lcd.setCursor(0,0);
  lcd.print("T:"+String(int(t))+"*C" + " Hum:" + String(int(h)) + "%-->");
  lcd.setCursor(0,1);
  lcd.print("Air Value: "); lcd.print(airValue); 
  */
  
  if(connectedUDP){
  
    int packetSize = UDP1.parsePacket();
    char value;
    if(packetSize){
      Serial.println();
      Serial.print("Received packet of size ");
      Serial.println(packetSize);
      Serial.print("From ");
      IPAddress remote = UDP1.remoteIP();
      for (int i =0; i < 4; i++) {
        Serial.print(remote[i], DEC);
        if (i < 3) {
            Serial.print(".");
        }
      }
      Serial.print(", port ");
      Serial.println(UDP1.remotePort());

      //read the packet UDP into buffer
      UDP1.read(packetBuffer, 1);
      Serial.println("Contents: ");
      value = packetBuffer[0];
      Serial.print(value);
    }
    switch(value){
      case '1':
        cmd = 1; break;
      case '2':
        cmd = 2; break;
      case '3':
        cmd = 3; break;
      case '0':
        cmd = 0; break;
    }
    if (cmd == 1) {
      Serial.println("Starting send UDP-packet");
      sendUDP();
    }
    if (cmd == 2) {
      Serial.println("Starting send TCP_packet");
      sendTCP();
    }
    if (cmd == 3) {
      Serial.println("Starting send MQTT-packet");
      sendMQTT();
    }
    delay(1000);
  } else connectUDP();
}

void connectWiFi(){
  Serial.println(); Serial.println();
  Serial.print("Connecting to "); Serial.print(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(D4, LOW);
}
 
void connectUDP(){
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("Connecting... to UDP");
    if(UDP1.begin(localPort) == 1){
      Serial.println("Connection successful");
      connectedUDP = true;
    }
    else {
      connectedUDP = false;
      Serial.println("Connection failed");
    }
  }
  
}

void sendUDP(){
  char data[50];
  for (int i = 0; i < 50; i++){
    data[i] += genRandom();
  }
  UDP2.beginPacket(server,portUDP);
  Serial.print("Sending UDP-packet.....^ ");
  Serial.println(data);
  UDP2.write(data);
  UDP2.endPacket();
  delay(100);
}

void reconnectTCP(){
  while (!clientTCP.connected()){
    Serial.println("Connecting... to TCP-server");
    if (clientTCP.connect(server, portTCP)){
      delay(1000);
      Serial.println("Connected to TCP-server");
    } else {
      delay(1000);
      Serial.println("Failed connect to TCP_server");
    }
  }
}

void sendTCP(){
  if(!clientTCP.connected()){
    reconnectTCP();
  }
  else {
    clientTCP.print(randomString(50));
    Serial.print("Sending TCP-packet...:");
    Serial.println(randomString(50));
    delay(100);
  }
}

void reconnectMQTT() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("node13")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      //client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 1 seconds");
      // Wait 1 seconds before retrying
      delay(1000);
    }
  }
}
void sendMQTT(){
  if (!client.connected()){
    reconnectMQTT();
  }
  Serial.print("Publish message: ");
  char data[50];
  for (int i = 0; i < 50; i++){
    data[i] += genRandom();
  }
  Serial.println(data);
  client.publish("Node13", data);
}
