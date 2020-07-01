#include<reg52.h>
sbit LED = P0^0;//tell the MCU that LED pin is the first SFR first pin which is P0.0
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

//ENLED,ADDR3,ADDR2,ADDR1 and ADDR0 five pins on the board, their values on the truth table can determine if the LES6 is outputing a high voltage
//when LES6 is outputing a high voltage, the triode will be enabled and the e and c terminals on the triode will be connected which means the circiut is close
//therefore LED0 set to be 0 makes the voltage difference 5V in result of LED0 being light up
void main()
{
	ENLED = 0;
	ADDR3 = 1;//there are 3 jump wire on P1.0,1.1,1.2,1.3 therefore we need to define AADR0 to 3
	ADDR2 = 1;
	ADDR1 = 1;
	ADDR0 = 0;
	
	while(1)
	{
		LED = 0;
	}
	//better way to make a infinite loop
	//LED = 0;
	//while(1); this makes the MCU keeps checking condition without going forward
	
}

