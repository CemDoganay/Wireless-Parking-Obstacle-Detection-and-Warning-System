#include <RF24.h>
#include <SPI.h>
#define battery_input A0

RF24 radio(7, 8);               

const byte address[6] = "00001";

const int trigPin_L = 10;  
const int echoPin_L = 9;

const int trigPin_M = 4;
const int echoPin_M= 5;

const int trigPin_R = 2;
const int echoPin_R = 3;

byte Array[7];

double critical;
long critical_sens;

const long node = 4;

void setup() 
{
radio.begin();
radio.openWritingPipe(address);
radio.setPALevel(RF24_PA_MIN);
  
pinMode(trigPin_L, OUTPUT); 
pinMode(echoPin_L, INPUT);

pinMode(trigPin_M, OUTPUT); 
pinMode(echoPin_M, INPUT); 

pinMode(trigPin_R, OUTPUT); 
pinMode(echoPin_R, INPUT);

Serial.begin(9600);
}

void loop() 
{
  
int x = analogRead(battery_input);
int battery = x * (5.0 / 1023.0);
if(battery<3)
{
  battery = 0;
}
else
{
  battery = 1;
}

  digitalWrite(trigPin_L, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_L, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_L, LOW);
  
 long duration_L = pulseIn(echoPin_L, HIGH);
 int distance_L= duration_L*0.034/2;

  digitalWrite(trigPin_M, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_M, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_M, LOW);

 long duration_M = pulseIn(echoPin_M, HIGH);
 int distance_M= duration_M*0.034/2;

  digitalWrite(trigPin_R, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_R, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_R, LOW);

 long duration_R = pulseIn(echoPin_R, HIGH);
 int distance_R= duration_R*0.034/2;

 Serial.println(distance_L);
 Serial.println(distance_M);
 Serial.println(distance_R);
 

  if (distance_L <= distance_M && distance_L <= distance_R) 
  {
    critical = distance_L;
    critical_sens = 1;
  } 
  else if (distance_M <= distance_R && distance_M <= distance_L) 
  {
    critical = distance_M;
    critical_sens = 2;
  } 
 else 
  {
   critical = distance_R;
    critical_sens = 3;
  }

  Array[0] = node;
  Array[1] = critical_sens;
  Array[2] = critical;
  Array[3] = distance_L;
  Array[4] = distance_M;
  Array[5] = distance_R;
  Array[6] = battery;

  Serial.print("battery status: ");
  Serial.println(Array[6]);
  
  Serial.print("distance_R: ");
  Serial.println(Array[5]);
  
  Serial.print("distance_M: ");
  Serial.println(Array[4]);
  
  Serial.print("distance_L: ");
  Serial.println(Array[3]);
  
  Serial.print("critical length: ");
  Serial.println(Array[2]);
  
  Serial.print("critical sensor: ");
  Serial.println(Array[1]);
  
  Serial.print("unit: ");
  Serial.println(Array[0]);
  
  radio.write(&Array, sizeof(Array));

  delay(random(500));
}
