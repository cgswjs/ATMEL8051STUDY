#include<reg52.h>

//this is a code that use interrupt instead of delay
unsigned long beats = 0;

void StartMotor(unsigned long angle);
void main()
{
    EA=1;//turn on intterruptor
    TMOD = 0x01;//choose mode 2 timer
    TH0 = 0xF8;
    TL0 = 0xCD;
    ET0 = 1;//turn on ET0 interrupter
    TR0 = 1;//turn on timer 0

    StartMotor(360*2+180);
    while(1);
}

void StartMotor(unsigned long angle)
{
    //turn off the interrupter can avoid the beats value being changed in the interrupt function
    //since beat is a long type it takes 4 times to finish calculating 
    EA = 0;
    beats = (angle*4076)/360;
    EA = 1;
}

void InterruptTimer0() interrupt 1
{
    unsigned char tmp;
    static unsigned char index = 0;
    unsigned char code BeatCode[8]]{
        0x0E,0x0c,0x0D,0x09,0x0B,0x03,0x07,0x06};

    TH0 = 0xF8;
    Tl0 = 0xCD;

    if(beats != 0 )
    {
        tmp = P1;
        tmp = tmp & 0xF0;
        tmp = tmp | BeatCode[index];
        P1 = tmp;
        index++;
        index = index & 0x07;
        beats --;
    }
}