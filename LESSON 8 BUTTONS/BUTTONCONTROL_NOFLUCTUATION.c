#include<reg52.h>

//BUTTON CONTROL V2 DEAL WITH MECHANICAL FLUCTUATION
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

//get LED pins
sbit LED9 = P0^7;
sbit LED8 = P0^6;
sbit LED7 = P0^5;
sbit LED6 = P0^4;

//get button pins
sbit KEY1 = P2^4;
sbit KEY2 = P2^5;
sbit KEY3 = P2^6;
sbit KEY4 = P2^7;

//create an array to record display lights status for each press of the button
unsigned char code LedChar[]={
    0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E
};

void delay();
void main()
{
    bit backup = 1;//bit can only be used in 8051, when backup is 1 button is not pressed
    bit keybuf = 1;//save previous status
    unsigned char cnt = 0;

    ENLED = 0;//low voltage
    ADDR3 = 1;//high voltage

    ADDR2 = 0;
    ADDR1 = 0;
    ADDR0 = 0;

    P2 = 0xF7;//this makes the first row of button work 11110111 <in1 in2 in3 in4 out1 out2 out3 out4>
     
    P0 = LedChar[cnt];//get display lights status for current press order

    while(1)
    {
        keybuf = KEY4;//let keybuf equal to current status of KEY4
        if(keybuf != backup)//check if current status is same as previuos status. Delay 10ms and check again if they are not
        {
            delay();//delay 10ms to avoid mechanical fluctuation
            if(keybuf == KEY4)//check again if keybuf is equal to currnet KEY4. If they are, that means someone really pushed the button
            {
                if(backup ==0)
                {
                    cnt++;
                    if(cnt>=10)
                    {
                        cnt = 0;
                    }
                }
                P0 = LedChar[cnt];//change display lights status based on how many times we press 
            }
            backup = KEY4;//reassign backup to current status of KEY4
        }

        LED9 = KEY1;
        LED8 = KEY2;
        LED7 = KEY3;
        LED6 = KEY4;
    }
}

//delay is a bad way to deal with fluctuation in real design
void delay()
{
    //this make sthe board calculate nhothing for 10ms
    unsigned int i =1000;
    while(i--);
}