// A4 – SDA
// A5 – SCL
#include<Wire.h> // I2C library
#include <Servo.h>
#include <Ultrasonic.h>

int instruction[5] = {5,0,0,0,0};

#define TRIGGER_PIN  7
#define ECHO_PIN     8

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

Servo temp_servo;
int temp_sensor = 0;
long microsec; //tempo HCSR04

float batmin=10.5;

void setup()
{
  Wire.begin(0x04); // set the slave address
  Wire.onRequest(requestEvent); // Sending information back to the NXT/EV3
  Wire.onReceive(receiveI2C); // Receiving information!

  // Debugging
  Serial.begin(9600);//ou 9600 ou 57600
}

float cmMsec;
void loop()
{
  microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);

  delay(50);
}

byte read_byte = 0x00;
int byte_count = 0;

// When data is received from NXT/EV3, this function is called.
void receiveI2C(int bytesIn)
{ 
  read_byte = bytesIn;
  byte_count = 0;
  while(1 < Wire.available()) // loop through all but the last
  {
    read_byte = Wire.read(); 
    
    instruction[byte_count] = read_byte;
    
    byte_count++;
  }
  int x = Wire.read(); // Read the last dummy byte (has no meaning, but must read it)


  if( instruction[0] == 1 ) //LED  
  {
    Serial.println("  Light ");
    
    Serial.print("Pin: "); 
    Serial.println(instruction[1]);
    pinMode(instruction[1], OUTPUT); //pino
    
    Serial.print("State: ");
    if(instruction[2] == 0) 
    {
      Serial.print("off");
      digitalWrite(instruction[1], LOW); //desliga
    }
    else                    
    {
      Serial.println("on");
      digitalWrite(instruction[1], HIGH); //liga
    }       
  }
  else 
  
  if( instruction[0] == 2 ) //servo
  {
    Serial.println("  Servo Motor ");
    
    Serial.print("Pin: "); 
    Serial.println(instruction[1]);
    temp_servo.attach(instruction[1]); //pino
    
    instruction[2] = instruction[2]*2 + instruction[3]; //converte ângulo
    
    Serial.print("Angle: ");
    Serial.println(instruction[2]);
    temp_servo.write(instruction[2]); //ângulo
    
  }
  else 
  
  if( instruction[0] == 3 )  //motor DC
  {
    Serial.println("  DC Motor ");
    
    Serial.print("Pin1: "); 
    Serial.print(instruction[1]);
    pinMode(instruction[1], OUTPUT); //pino 1
    
    Serial.print("  Speed1: "); 
    Serial.println(instruction[2]);
    analogWrite(instruction[1], instruction[2]*2.55); //velocidade 1
    
    Serial.print("Pin2: "); 
    Serial.print(instruction[3]);
    pinMode(instruction[3], OUTPUT); //pino 2
    
    Serial.print("  Speed2: "); 
    Serial.println(instruction[4]);
    analogWrite(instruction[3], instruction[4]*2.55); //velocidade 2
    
    Serial.print("Result movement: "); // movimento resultante
    if (instruction[2] !=0)
      Serial.println("Forward"); // pra trás
    else
      Serial.println("Backwards"); // pra frente
  }
  else 
  
  if( instruction[0] == 4 )  //sensor
  {
    Serial.println("  Sensor "); 
    
    Serial.print("pino: "); 
    Serial.print(instruction[1]);
    
    if ( instruction[1] == 8 ){ //pino do sensor
      Serial.println(" Ultrassônico");
      temp_sensor = int(cmMsec);
       
    }else{
      Serial.print("nenhum sensor detectado...");  
    }
  }else{
    Serial.print("nada por enquanto...");  
  }
   
}//end recieveI2C

void requestEvent()
{  
  if (instruction[0] == 4)
  {
    Wire.write(temp_sensor); // respond with message
    Serial.print("Value: ");
    Serial.println(temp_sensor);
  } 
}//end requestEvent
