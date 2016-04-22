#include <SPI.h>
#include <Ethernet.h>

// MAC-адрес нашего устройства 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };    

// ip-адрес удалённого сервера 
byte server[] = { 13, 92, 80, 101 }; // измени на свой

int const ledPin = 8;

EthernetClient client;

void printIPAddress()
{
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }
  Serial.println();
}

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // this check is only needed on the Leonardo:
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for (;;)
      ;
  }
  delay(1000);
  // print your local IP address:
  Serial.println(Ethernet.localIP());

  pinMode(ledPin, OUTPUT); // configure LED-PIN.

   //Connect to server, port 4000.
  if(client.connect(server, 4000)) {
    Serial.print("connected");
  }
  else {
    Serial.println("Connection failed");
  }
  
  client.write("A28F26"); // send data to server
}

void loop() {
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
    switch (c){
      case 49:  // if char c = 1
        digitalWrite(ledPin, HIGH);
        Serial.println("LED ON");
      case 48:  // if char c = 0
        digitalWrite(ledPin, LOW);
        Serial.println("LED OFF");
    }
  }

  if (!client.connected()) {
    Serial.println();
    //Serial.println("disconnecting.");
    //client.stop();
    //for(;;)
    //;
    Serial.println("Connecting....................");
    if(client.connect(server,80)){
      Serial.println("Connected");
    }
    else
      Serial.println("Connection failed");
  }
}
