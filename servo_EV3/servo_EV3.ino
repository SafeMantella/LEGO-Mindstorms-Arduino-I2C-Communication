#include <Wire.h>
#include <Servo.h>
 
#define SLAVE_ADDRESS 0x04
#define SERVO_PIN        9
 
Servo srv;
 
void setup()
{
    Serial.begin(9600);         // start serial for output
    Wire.begin(SLAVE_ADDRESS);
    Wire.onReceive(receiveData);
    Wire.onRequest(sendData);//[EXPERIMENTO]
    srv.attach(SERVO_PIN);
}

int val,flag=0;//[EXPERIMENTO] 
void loop()
{
  if(flag==1)
   {
     Serial.print(val);// val é o valor recebido pelo arduino, o valor não é alterado em seu processamento
     flag=0;
   }//[EXPERIMENTO]
}
 
void receiveData(int byteCount)
{
    while(Wire.available()>0)
    {
      val=Wire.read();//[EXPERIMENTO], se der errado, substitui val por Wire.read 
      flag=1;//[EXPERIMENTO] 
      
      srv.write(val);//gira servo
      Serial.print(val);//printa graus no monitor serial
    }
}

// callback for sending data [EXPERIMENTO]
void sendData()// 0 = 0 , 30 = 1E , 60 = 3C , 90 = 5A
{
  Wire.write(0x46);//número após 0x é alterado para aparecer no sensor - O número é convertido em hexadecimal
}
