#include<reg52.h>

//get pins for 74HC138 
sbit ENLED = P1^4;//enable P1
sbit ADDR0 = P1^0;//change different display light
sbit ADDR1 = P1^1;//change ~
sbit ADDR2 = P1^2;//change ~
sbit ADDR3 = P1^3;//enable P1

//initial values for display
unsigned char code LedChar[] = {
    0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E
};

//there are 6 display lights and initially all 0 turned off
unsigned char LedBuff[6]={
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
}
 //since char can only go to 255, we need int here to count more
unsigned int cnt = 0;
unsigned long sec = 0;
unsigned char i = 0;

void main()
{
   
    ENLED = 0;
    ADDR3 = 1;
    TMOD = 0x01;
    TH0 = 0xFC;
    TL0 = 0x67;
    TR0 = 1;
    EA = 1;//turn interrupt switch on
    ET0 = 0;//turn timer0 interrupter switch on 

    while(1)
    {
        if(cnt>=1000)
        {
            cnt = 0;//when count is 1000 delay is 1sec, reset cnt
            sec++;//increase sec by 1
        
            LedBuff[0] = LedChar[sec%10];
            LedBuff[1] = LedChar[sec/10%10];
            LedBuff[2] = LedChar[sec/100%10];
            LedBuff[3] = LedChar[sec/1000%10];
            LedBuff[4] = LedChar[sec/10000%10];
            LedBuff[5] = LedChar[sec/100000%10];
        }
        
    }
}

//hardware reset TF to 0 whenever TF overflow
//interrupt number is calclated by x*8+3 = 000B(11)<interrupter address>
//therefore interrupt number x=1
void InterruptTimer0() interrupt 1
{
    TH0 = 0xFC;
    TL0 = 0X67;
    cnt++;
    P0 = 0xFF;

    switch(i)
        {
            case 0 : ADDR2 = 0;ADDR1 = 0;ADDR0 = 0;i++;P0 = LedBuff[0];break;
            case 1 : ADDR2 = 0;ADDR1 = 0;ADDR0 = 1;i++;P0 = LedBuff[1];break;
            case 2 : ADDR2 = 0;ADDR1 = 1;ADDR0 = 0;i++;P0 = LedBuff[2];break;
            case 3 : ADDR2 = 0;ADDR1 = 1;ADDR0 = 1;i++;P0 = LedBuff[3];break;
            case 4 : ADDR2 = 1;ADDR1 = 0;ADDR0 = 0;i++;P0 = LedBuff[4];break;
            //number 6 display is the last digital light so we set i back to 0
            case 5 : ADDR2 = 1;ADDR1 = 0;ADDR0 = 1;i=0;P0 = LedBuff[5];break;
            default: break;
        }
}

//software reset could lead to fluctuation
//hardware reset is better