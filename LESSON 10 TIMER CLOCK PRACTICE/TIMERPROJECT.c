#include<reg52.h>

//get all pins
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
sbit KEY1 = P2^4;
sbit KEY2 = P2^5;
sbit KEY3 = P2^6;
sbit KEY4 = P2^7;

//get different display lights code
unsigned char code LedChar[]={
    0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E
};

//get buffer area code
unsigned char LedBuff[6]={
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

//get current status of key
unsigned char KeySta[4]={
    1,1,1,1
};

//state any function used in main
void KeyDriver();

//MAIN
unsigned char T0RH=0;//initial T0RH
unsigned char T0RL=0;//initial T0RL
bit StopwatchRefresh = 1;//display status of stopwatch not display
bit StopwatchRunning = 0;//initial status of stopwatch not running
unsigned char DecimalPart = 0;//decimal part of display
unsigned int IntegerPart = 0;//integer part of display

void ConfigTimer0(unsigned int delay);
void StopwatchDisplay();
void StopwatchReset();
void StopwatchAction();
void KeyDriver();
void StopwatchCount();

void main()
{
    //turn on interruptor
    EA = 1;
    ENLED = 0;
    ADDR3 = 1;
    P2 = 0xFE;//turn row 4 on which mean P2.0=0 so we make P2 = 11111110 to turn the last pin on

    //use ConfigTimer0 to configure the timer0 to delay 2ms
    ConfigTimer0(2);
    while(1)
    {
        if(StopwatchRefresh)
        {
            StopwatchRefresh = 0;//set Stopwatch display on
            StopwatchDisplay();
        }
        KeyDriver();
    }
}

//ConfigTimer0 function
void ConfigTimer0(unsigned int delay)
{
    unsigned long tmp;//define local variable for this function
    tmp = 11059200/12;//frequency of 1 machine cycle since f=1/T
    tmp = (tmp*delay)/1000;// target delay time/f equals the total cycle needed to achieve the target delay time
    tmp = 65536-tmp;//calculate the start point of the timer
    tmp = tmp+18;//time compensation

    T0RH = (unsigned char)(tmp >> 8);//get the first 8 digit
    T0RL = (unsigned char)tmp;//get the low 8 digit

    //since TMOD = 0x01 give 0000 0001, this will affect the timer1, therefore we want to use timer0 without interference timer1
    //use &= to make the lower four digit all 0 0xF0 = 11110000
    //&1=1,&0=0,|1=1,|0=original value
    TMOD &= 0xF0;//clear the lower 4 digit of TMOD to all 0
    TMOD |= 0x01;//since 0x01=0001, for TMOD 0001 will be convert to a 8bit 00000001, |= makes the last digit 1 which is 11110001

    TH0 = T0RH;//set TH0
    TL0 = T0RL;//set TL0
    ET0 = 1;//turn on overflow interrupt
    TR0 = 1;  //turn on timer0
}

//StopwatchDisplay function
void StopwatchDisplay()
{
    signed char i;
    unsigned char buf[4];//buffer that stores interger part
    LedBuff[0]=LedChar[DecimalPart%10];
    LedBuff[1]=LedChar[DecimalPart/10];
    buf[0]=IntegerPart%10;//first digit of int
    buf[1]=IntegerPart/10%10;//2nd
    buf[2]=IntegerPart/100%10;//3rd
    buf[3]=IntegerPart/1000%10;//4rd
    //check from the left side to the right side to see if the first digit is 0
    for(i=3;i>=1;i--)
    {
        if(buf[i]==0)
        {
            //if first digit is 0, turn off the display of this digit
            LedBuff[i+2]=0xFF;
        }
        else
        {
            break;
        }
        
    }
    for(;,i>=0;i--)
    {
        LedBuff[i+2]=LedChar[buf[i]];//since there are 2 decimal, we need to put buf[i] to LedBuff[i+2]
    }
    LedBuff[2] &= 0x7F;//show decimal point on the LedBuff[2] display light


}

//StopwatchRest
void StopwatchReset()
{
    StopwatchRunning = 0;
    DecimalPart = 0;
    IntegerPart = 0;
    StopwatchRefresh = 1;
}

//StopwatchAction
void StopwatchAction()
{
    if(StopwatchRunning)
    {
        StopwatchRunning = 0;//if stopwatch is running stop it
    }
    else
    {
        StopwatchRunning = 1;//otherwise, start it
    }
    
}

//KeyDriver function
void KeyDriver()
{
    unsigned char i;//define i for for loop
    static unsigned char backup[4]={1,1,1,1};//create backup to store status of each key
    for(i=0;i<4;i++)
    {
        if(backup[i] != KeySta[i])
        {
            if(backup[i]!= 0)
            {
                //since we only use key 1 and key 2 which corresponding to the reset and action key(K14,K15)
                if(i == 1)
                {
                    //when key 14 is pressed stop watch
                    StopwatchReset();
                }
                else if(i==2)
                {
                    //when key 15 is pressed start watch
                    StopwatchAction();
                }
            }
            backup[i] = KeySta[i];
        }
    }
}
//StopwatchCount
void StopwatchCount()
{
    if(StopwatchRunning)
    {
        //when this function is called to the interrupt, that means 10ms has passed
        //so decimal need to increase by 1
        DecimalPart++;
        //when DecimalPart increase by 100, it means 10msx100=1s has been added
        if(DecimalPart>=100)
        {
            //set DecimalPart to 0 again to restart counting
            DecimalPart = 0;
            //add 1s to integer part
            IntegerPart++;
            //since we only have four display lights, can't exceeed 9999
            if(IntegerPart>=10000)
            {
                //set IngegerPart to 0 when overflow
                IntegerPart = 0;
            }
        }

        //turn off stopwatch display which makes the main function turn it back on again to refresh the readings
        StopwatchRefresh = 1;
    }
}

//interrupt
void InterruptTimer0() interrupt 1
{
    static unsigned char tmr10ms = 0;//use tmr10ms to keep track how many times the interrupt has been processed
    TH0 = T0RH;
    TL0 = T0RL;
    LedScan();
    KeySan();

    tmr10ms++;
    //when 5 interrupt is processed 5 times, delay is 10ms since single time delay is 2ms 
    //Then stopwatch count will be ran to update the last decimal digit 
    if(tmr10ms>=5){
        tmr10ms=0;
        StopwatchCount();
    }
}
