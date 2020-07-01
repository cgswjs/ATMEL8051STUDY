#include<reg52.h>

//this code will control a no resource buzzer
//no resource buzzer needs a input pulse to sound
sbit BUZZ = P1^6;

unsigned char T0RH = 0;
unsigned char T0RL = 0;

void OpenBuzz(unsigned int freq);
void StopBUzz();

void main()
{
    unsigned int i;
    EA = 1;
    TMOD = 0x01;
    
    while(1)
    {
        OpenBuzz(4000);
        for(i=0;i<40000;i++);
        StopBuzz();
        for(i=0;i<40000;i++);
        OpenBuzz(1000);
        for(i=0;i<40000;i++);
        StopBUzz();
        for(i=0;i<40000;i++)
    }
}

void OpenBuzz(unsigned int freq)
{
    unsigned int reload;//create a 16bits number to record the time for a cycle
    reload = 65536-(11059200/12)/(freq*2);// x*12/1159200=1/(2*freq) ----->x is the total machine cycle for one pulse(one pulse contains both high and low amp so delay time is 1/2f)
    T0RH = (unsigned char)(reload >>8);//force type convert to get the high 8 bits
    T0RL = (unsigned char)reload;//same way to get low 8 bits
    
    TH0 = 0xFF;//give TH0 a random high value
    TL0 = 0xFE;//same for TL0

    ET0 = 1;
    TR0 = 1;
}

void StopBuzz()
{
    ET0 = 0;//turn off intterrupt
    TR0 = 0;//turn off timer
}

void InterruptTimer0() interrupt 1
{
    TH0 = T0RH;
    TL0 = T0RL;
    BUZZ = ~BUZZ;
}