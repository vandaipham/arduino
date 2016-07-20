#include <SoftwareSerial.h>

SoftwareSerial mySerial(0, 1); // RX, TX

//variable for Joytick
int x = 0;
int y = 0;

int val_x = 0;
int val_y = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Hello everybody!");

   // set the data rate for the SoftwareSerial port
  mySerial.begin(115200);
  
}
 
void loop() {
  x = analogRead(A0);
  y = analogRead(A1);
  if(x <= 542 && x >= 482)
  {
     x = 0; 
  }
  else if(x < 482)
  {
    x = -(512 - x);     
  }
  else 
  {
    x = x - 511;
  }
//***************************************  
  if(y <= 542 && y >= 482)
  {
     y = 0; 
  }
  else if(y < 482)
  {
    y = -(512 - y);     
  }
  else 
  {
    y = y - 511;
  }
  
  val_x = map(x,0,512,0,255);
  val_y = map(y,0,512,0,255);
  
//  Serial.println(val_y);
//  Serial.println(val_y);
//******************************************
  delay(200);
  String cmd_x = "tx_test " + val_x;
  sendCmd(cmd_x, 1000, true);
  String cmd_y = "tx_test " + val_y;
  sendCmd(cmd_y, 1000, true);
}

String sendCmd(String command, const int timeout, boolean debug){
  String response = "";
  mySerial.print(command); // send command to the Lora.
  long int time = millis();
  while((time+timeout) > millis()){
    while(mySerial.available()){
      //the lora has data so display its output to the serial window
      char c = mySerial.read(); //read the next character
      response += c;
    }
  }
  if(debug){
    Serial.print(response);
  }
  return response;
}

