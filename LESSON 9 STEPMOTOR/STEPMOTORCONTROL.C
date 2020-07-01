#include<reg52.h>

//this is a simple code that can control the step motor

void TrunMotor(unsigned long angle);
void main()
{
    TurnMotor(360*25);
    while(1);
    
    
}

void delay()
{
    unsigned int i=200;
    while(i--);
}
void TrunMotor(unsigned long angle)
{
    unsigned char tmp;
    unsigned char index = 0;
    unsigned long beats = 0;
    unsigned char code BeatCode[8]={
    0x0E,0x0c,0x0D,0x09,0x0B,0x03,0x07,0x06};
    beats = (angle*4076)/360;
    while(beats--)
    {
        tmp = P1;
        tmp = tmp & 0xF0;
        tmp = tmp | BeatCode[index];
        P1 = tmp;
        index++;
        index = index & 0x07;//when index is 7 it will be cleared to 0 (& operator in C)
        delay();
    }

}
