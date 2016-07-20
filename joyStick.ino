
const int SW = 2;
const int VRx = A0;
const int VRy = A1;

const int led = 13;
int x = 0;
int y = 0;

int val_x = 0;
int val_y = 0;

void setup() {
  
  pinMode(led, OUTPUT);
  pinMode(SW, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  if(digitalRead(SW) == LOW)
    digitalWrite(led, HIGH);
  else
    digitalWrite(led, LOW);
    
  x = analogRead(VRx);
  y = analogRead(VRy);
  
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
  if(val_x > 0)
  {
    analogWrite(5, 0);
    analogWrite(6, val_x);  
  }
  else if(val_x < 0)
  {
    analogWrite(5, abs(val_x));
    analogWrite(6, 0);  
  }
  else
  {
    analogWrite(5, 0);
    analogWrite(6, 0);     
  }
//******************************************  
  if(val_y > 0)
  {
    analogWrite(10, 0);
    analogWrite(11, val_y);  
  }
  else if(val_y < 0)
  {
    analogWrite(10, abs(val_y));
    analogWrite(11, 0);  
  }
  else
  {
    analogWrite(10, 0);
    analogWrite(11, 0);     
  }

}
