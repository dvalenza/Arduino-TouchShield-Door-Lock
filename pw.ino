/* 
 Touch Shield Example using the MPR121 touch sensor IC 

 by: Aaron Weiss, based on the MPR121 Keypad Example by Jim Lindblom
     
 SparkFun Electronics
 created on: 5/12/11
 license: OSHW 1.0, http://freedomdefined.org/OSHW
 
 Pressing a pad will print the corresponding number.
 
 Hardware: 3.3V or 5V Arduino

 Notes: The Wiring library is not used for I2C, a default atmel I2C lib
        is used. Be sure to keep the .h files with the project. 
*/

// include the atmel I2C libs
#include "mpr121.h"
#include "i2c.h"
#include <Servo.h> 
// 11 max digits used
#define DIGITS 11 

// Match key inputs with electrode numbers
#define ONE 8
#define TWO 5
#define THREE 2
#define FOUR 7
#define FIVE 4
#define SIX 1
#define SEVEN 6
#define EIGHT 3
#define NINE 0

//extras (not used)
#define ELE9 9
#define ELE10 10
#define ELE11 11

//interupt pin
int irqpin = 2;  // D2

Servo myservo;
int pos = 0;

void setup()
{
  //make sure the interrupt pin is an input and pulled high
  pinMode(irqpin, INPUT);
  digitalWrite(irqpin, HIGH);
  myservo.attach(9);
  //configure serial out
  Serial.begin(9600);
  //output on ADC4 (PC4, SDA)
  DDRC |= 0b00010011;
  // Pull-ups on I2C Bus
  PORTC = 0b00110000; 
  // initalize I2C bus. Wiring lib not used. 
  i2cInit();
  
  delay(100);
  // initialize mpr121
  mpr121QuickConfig();
  
  // Create and interrupt to trigger when a button
  // is hit, the IRQ pin goes low, and the function getNumber is run. 
  attachInterrupt(0,getNumber,LOW);
  
  // prints 'Ready...' when you can start hitting numbers
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  Serial.println("Ready...");
}

void loop()
{


}
void blinker(){
  for(int x = 0; x<=3; x++){    
    on();
    delay(1000);
    off();
    delay(1000);
  }
}        

void on(){
 digitalWrite(13, HIGH);
}
void off(){
 digitalWrite(13, LOW);
}
void ony(){
 digitalWrite(12, HIGH);
}
void offy(){
 digitalWrite(12, LOW);
}

int pw = 123;
int inp;
int i = 0;
char digits[DIGITS];
void getNumber()
{
  int touchNumber = 0;
  uint16_t touchstatus;
  
  touchstatus = mpr121Read(0x01) << 8;
  touchstatus |= mpr121Read(0x00);
  
  for (int j=0; j<12; j++)  // Check how many electrodes were pressed
  {
    if ((touchstatus & (1<<j)))
      touchNumber++;
  }
  
  if (touchNumber == 1)
  {
    if (touchstatus & (1<<SEVEN))
      digits[i] = '7';  
    else if (touchstatus & (1<<FOUR))
      digits[i] = '4';
    else if (touchstatus & (1<<ONE)){
      digits[i] = '1'; i=0;}
    else if (touchstatus & (1<<EIGHT))
      digits[i] = '8';
    else if (touchstatus & (1<<FIVE))
      digits[i] = '5';
    else if (touchstatus & (1<<TWO))
      digits[i] = '2';
    else if (touchstatus & (1<<NINE))
      digits[i] = '9';
    else if (touchstatus & (1<<SIX))
      digits[i] = '6';
    else if (touchstatus & (1<<THREE))
      digits[i] = '3';
      
    Serial.print(digits[i]);
    i++;
    if (touchstatus & (1<<TWO)){
      i=0;
    }
    if(i==4){
      Serial.print("i4!");
      i=0;
      Serial.print("Reset!\n");
      blinker();
    }  
  }

      if(digits[0] == '1' && digits[1] == '9' && digits[2] == '8' && digits[3] == '7'){
        blinker();
        myservo.write(42);    
        i=0;
        Serial.print("Unlocked!");
      }
      if(digits[0] == '3' && digits[1] == '3' && digits[2] == '3' && digits[3] == '3'){
        blinker();
        myservo.write(140);
        i=0;
        Serial.print("Locked!");
      }
  else if (touchNumber == 2){
  if (touchstatus & (1<<SEVEN)){
      blinker();
      myservo.write(90);
      i=0;
  }
    ;
  }
  else
    ;
}
