#include <reg51.h>

#define endTime 100 //timer = 0.2ms -> endTime = 100*0.2ms = 20ms
#define zero 3      //3*0.2ms = 0.6ms -> 0º
#define max 12      //12*0.2ms = 2.4ms -> 180º
#define adder 1     //will be added to each angle
#define delay 4     //Sets the speed
//Variable declaration
sbit servo = P1^0;  //Pin that controls the motor
sbit led = P1^1;    //Pin that controls the LED
sbit sensor = P3^2; //Pin that recieves the interruption from Light Sensor
unsigned int counter = 0;    //Will increment every 2ms
unsigned int counter2 = 0;
unsigned char servoIncrementer = zero; //controls the servo pulse
signed char adderVar = adder;

//Function declaration:

void begin(){
  EA = 1;  //Enables the ability to recieve external interruptions
  ET0 = 1;    //Enables interruptions from timer 0
  EX0 = 1;    //Enables interruptions from P3^2 (sensor)
  //Timer mode
  TMOD &= 0xF0; //Cleans all 3 bits from timer0
  TMOD |= 0x02; //Sets the timer to mode 2 this allows the auto reload
  //Timer configuration
  TH0 = TL0 = 0x37; //Set clocks to reset every 2ms
  /*To achieve 2ms:
      The timer overflows at 0xFF -> 255
      We need to count 200µs wich is equivalent to 0xC8 -> 200
      So: 0xFF - 0xC8 = 0x37 = 55
      Wich means the clock must start at 55ms everytime so it overflows at 200ms*/
  IT0 = 0; //Acivates external interruption at falling edge
	TR0 = 1; //Starts the timmer 0
}

/**
  * External interruption, this is active when the light sensor finds Light
  */
void External0_ISR() interrupt 0{
	led = 0;
}

/**
* Timer interruption, activates everytime the clock overflows
*/
void Timer0_ISR() interrupt 1{
  counter++;
}

void main(){
  begin();
  while(1){
    if(led == 1){
      if(counter == endTime){
        servo = 1;  //Starts the pulse
        counter = 0;  //Restarts counting
        counter2++;
      }

      if(counter == servoIncrementer)
        servo = 0;  //stops the pulse
    }
    if(counter2 == delay){
      counter2 = 0;
			servoIncrementer += adderVar;
      if(servoIncrementer <= zero || servoIncrementer >= max)
        adderVar = adderVar * (-1);
    }
    led = 1; //Keeps LED powered off
  }
}
