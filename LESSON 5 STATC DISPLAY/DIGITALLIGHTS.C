#include<reg52.h>

//THIS IS AN EXAMPLE OF STATIC DISPLAY, ONLY ONE DIGITAL LIGHT CAN BE LIGHT UP AT A TIME
//A digital light is used to show numbers on a 8051 board
//There are 7 segments of this digital light and 8 pins to control them
//DB0 to DB6 pins control segment 1 to g, DB7 controls dp which is the decimal point
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

//record hex code that lights different digital lights
unsigned char LedChar[] = {
    0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E
};

void main()
{
    unsigned char cnt = 0;//counter for overflow
    unsigned char sec = 0;//time clock
    
    ENLED = 0;
    ADDR3 = 1;
    ADDR2 = 0;
    ADDR1 = 0;
    ADDR0 = 0;

    //use TMOD to delay for digital light
    TMOD = 0x01;//select 2nd mode of the TIMERS
    TH0 = 0xB8;//set initial value of TH0 timer
    TL0 = 0x00;//set initial value of TL0 timer
    TR0 = 1;//turn on the timer
    while(1)
    {
        if(TF0 == 1)
        {
            TF0 = 0;
            TH0 = 0xB8;
            TL0 = 0x00;
            cnt++;
            if(cnt>=50)
            {
                cnt = 0;
                P0 = LedChar[sec];
                sec++;
                //since there are only 16 display in the LedChar array
                //sec need to be reset if it's larger than 15
                if(sec >= 16){
                    sec = 0;
                }
            }
        }
    }
    
    P0 = 0xF9;//we want b and c segments to light up, so we need 0b11111001 convert to hex
    while(1);
}