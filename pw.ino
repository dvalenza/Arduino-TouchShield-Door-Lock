

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
//PhotoResistor Pin
int lightPin = 0; //the analog pin the photoresistor is
                  //connected to
                  //the photoresistor is not calibrated to any units so
                  //this is simply a raw sensor value (relative light)
//LED Pin
int ledPin = 9;   //the pin the LED is connected to
                  //we are controlling brightness so
                  //we use one of the PWM (pulse width
                  // modulation pins)
Servo myservo;

void setup()
{
  //make sure the interrupt pin is an input and pulled high
  pinMode(irqpin, INPUT);
  digitalWrite(irqpin, HIGH);
  myservo.attach(9);
  Serial.begin(9600);
  DDRC |= 0b00010011;
  PORTC = 0b00110000; 
  i2cInit();
  delay(100);
  mpr121QuickConfig();
  
  // Create and interrupt to trigger when a button
  // is hit, the IRQ pin goes low, and the function getNumber is run. 
  attachInterrupt(0,getNumber,LOW);
  
  // prints 'Ready...' when you can start hitting numbers
  pinMode(13, OUTPUT);
  pinMode(ledPin, OUTPUT); 
  Serial.println("Ready...");
}

void loop()
{
  int lightLevel = analogRead(lightPin); 
  lightLevel = map(lightLevel, 0, 900, 0, 255);
  lightLevel = constrain(lightLevel, 0, 255);
  analogWrite(ledPin, lightLevel); 
}

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
      blinker(3);
    }  
  }

      if(digits[0] == '1' && digits[1] == '9' && digits[2] == '8' && digits[3] == '7'){
        blinker(3);
        myservo.write(42);    
        i=0;
        Serial.print("Unlocked!");
      }
      if(digits[0] == '3' && digits[1] == '3' && digits[2] == '3' && digits[3] == '3'){
        blinker(3);
        myservo.write(140);
        i=0;
        Serial.print("Locked!");
      }
  else if (touchNumber == 2){
  if (touchstatus & (1<<SEVEN)){
      blinker(3);
      myservo.write(90);
      i=0;
  }
    ;
  }
  else
    ;
}

void blinker(int t){
  for(int x = 0; x<=t; x++){    
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

