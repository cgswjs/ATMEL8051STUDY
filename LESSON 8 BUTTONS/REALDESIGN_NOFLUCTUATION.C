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
bit KeySta = 1;

void main()
{
    bit backup = 1;//bit can only be used in 8051, when backup is 1 button is not pressed
    unsigned char cnt = 0;

    ENLED = 0;//low voltage
    ADDR3 = 1;//high voltage

    ADDR2 = 0;
    ADDR1 = 0;
    ADDR0 = 0;

    //use timer to delay 
    TMOD = 0xF8;
    TH0 = 0xFC
    TL0 = 0x67;
    ET0 = 1;
    TR0 = 1;

    P2 = 0xF7;//this makes the first row of button work 11110111 <in1 in2 in3 in4 out1 out2 out3 out4>
    P0 = LedChar[cnt];//get display lights status for current press order

    while(1)
    {
       if(KeySta != backup)
       {
           if(backup == 0 )
           {
               cnt++;
               if(cnt>=10)
               {
                   cnt = 0;
               }
               P0 = LedChar[cnt];
           }
           backup = KeySta;
       }
    }


}

void interruptTimer0() interrupt 1
{
    static unsigned char keybuf = 0xFF;

    TH0 = 0xFC;
    TL0 = 0x67;

    keybuf = (keybuf <<1) | KEY4;//move keybuf to left and compare the digit corresponding to KEY4 using a OR operator. This will keep doing for 16 times and give a HEX number
    //check the hex number, if it's all 0 then the button is pressed, if it's all 1 the button is released, otherwise nothing happened
    if(keybuf == 0x00)
    {
        KeySta = 0;
    }
    else if(Keybuf == 0xFF)
    {
        KeySta = 1;
    }
    else
    {
    }
}