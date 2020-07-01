#include<reg52.h>

//THIS IS AN EXAMPLE OF 8051 TIMERS
sbit LED = P0^0;
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

void main()
{
    unsigned char cnt=0;
    ENLED = 0;
    ADDR3 = 1;
    ADDR2 = 1;
    ADDR1 = 1;
    ADDR0 = 0;

    TMOD = 0x01;
    TH0 = 0xB8;//set initial time stamp for TH0
    TL0 = 0x00;
    TL0 = 1;//turn on the timer

    while(1)
    {
        //use the TH0 and TL0 to synchronize and delay specific time for LED
        //since 1 machine cycle is 12*(1/frequency), 8051 has a frequency of 11.0592MHz
        //Therefore if we want to count for 0.02s,we need to calculate how many machine cycle it needs
        //x*12*(1/110592)=0.02 ---> x=18432
        //since the timer on 8051 can only count to 65536,we assign initial vaue to T0
        //if we assign y to T0, then y+x must not exceed 65536. Thus, y has to be less or equal to 47104
        //47104 convert to hexdecimal is 0xB800
        //when TF=1, it overflows
        if(TF0==1)
        {
            TF0 = 0;//reset
            TH0 = 0xB8;//reassign
            TL0 = 0x00;//reassign
            cnt++;//every 20ms cnt+1
            //when cnt is equal to 50 which means 1sec
            if(cnt++>=50)
            {
                cnt = 0;
                LED = ~LED;//turn on if off, turn off if on
            }
        }

    }
}