#include<reg52.h>
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
sbit LED = P0^0;

unsigned char code image[]={

    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xC3,0xE7,0xE7,0xE7,0xE7,0xE7,0xC3,0xFF,
    0x99,0x00,0x00,0x00,0x81,0xC3,0xE7,0xFF,
    0x99,0x99,0x99,0x99,0x99,0x81,0xC3,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

void main()
{
    ENLED = 0;
    ADDR3 = 0;
    EA = 1;//turn on interrupt main switch
    TMOD = 0X01;//choose timer mode 2
    TH0=0xFC;//set TH0
    TL0=0x67;//set TL0
    ET0=1;//turn on ET0 interrupt switch
    TR0=1;
    while(1);
}

void interruptTimer0() interrupt 1
{
    static unsigned char i=0;
    static unsigned char index = 0;//use index to keep track of the start line
    static unsigned char tmr = 0;//time between each image

    TH0 = 0xFC;
    TL0 = 0x67;

    P0 = 0xFF;//set P0=11111111 so all lights will be turned off initially
    switch(i)
    {
        case 0 : ADDR2 = 0;ADDR1=0;ADDR0=0;i++;P0=image[index+0];break;
        case 1 : ADDR2 = 0;ADDR1=0;ADDR0=1;i++;P0=image[index+1];break;
        case 2 : ADDR2 = 0;ADDR1=1;ADDR0=0;i++;P0=image[index+2];break;
        case 3 : ADDR2 = 0;ADDR1=1;ADDR0=1;i++;P0=image[index+3];break;
        case 4 : ADDR2 = 1;ADDR1=0;ADDR0=0;i++;P0=image[index+4];break;
        case 5 : ADDR2 = 1;ADDR1=0;ADDR0=1;i++;P0=image[index+5];break;
        case 6 : ADDR2 = 1;ADDR1=1;ADDR0=0;i++;P0=image[index+6];break;
        case 7 : ADDR2 = 1;ADDR1=1;ADDR0=1;i=0;P0=image[index+7];break;
        default break;
    }

    tmr++;
    if(tmr>=250)
    {
        tmr=0;
        index++;
        if(index>=32){
            index = 0;
        }
    }
}