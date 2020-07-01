#include<reg52.h>
//PWM is a digit signal that control analog output
sbit PWNMOUT = P0^0;
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

unsigned char HighRH = 0;
unsigned char HighRL = 0;
unsigned char LowRH = 0;
unsigned char LowRL = 0;
unsigned char T1RH = 0;
unsigned char T1RL = 0;
unsigned long PeriodCnt = 0;

//MAIN
void ConfigPWM(unsigned int fr, unsigned char dc)
void ConfigTimer1(unsigned int ms)
void AdjustDutyCycle(unsigned char dc)

void main()
{
    unsigned int i;
    
    EA = 1;
    ENLED = 0;
    ADDR3 = 1;
    ADDR2 = 1;
    ADDR1 = 1;
    ADDR0 = 0;

  
     
    //ConfigPWN take in Hz and percentage of signal with this Hz
    ConfigPWM(100,10);//100Hz 10%
    ConfigTimer1(50);
    while(1)

}

//config timer1
void ConfigTimer1(unsigned int ms)
{
    unsigned long tmp;

    tmp = 11059200/12;//how many cycles in 1ms
    tmp = (tmp*ms)/1000;//number of cycles in xms
    tmp = 65536-tmp;//get the start time stamp for xms delay
    tmp = tmp+12;
    T1RH = (unsigned char)(tmp>>8);
    T1RL = (unsigned char)tmp;
    TMOD &= 0x0F;//clear high 4 digit in TMOD to 0
    TMOD |= 0x10;//make the 4th digit 1 so that we have 00011111 to turn mode 1 on timer1 on
    TH1 = T1RH;//set TH1 
    TL1 = T1RL;//set TL1
    ET1 = 1;//turn interrupt on
    TR1 = 1;//turn timer on
}

//config pwm
void ConfigPWM(unsigned int fr, unsigned char dc)
{
    unsigned int high, low;

    PeriodCnt = 11059200/12/fr;//calculate number of cycle needed
    high = (PeriodCnt*dc)/100;
    low = PeriodCnt-high;
    high = 65536-high+12;//compensation
    low = 65536 - low+12;//compensation
    HighRH = (unsigned char)(high >>8);
    HighRL = (unsigned char)high;
    LowRH = (unsigned char)(low >> 8);
    LowRL = (unsigned char)low;
    TMOD &= 0xF0;//clear lower 4 digit in TMOD to 0
    TMOD |= 0x01;//make the last digit of 8 bit 1 so we have 11110001 to turn mode 1 on timer0 on
    TH0 = HighRH;
    TL0 = HighRL;
    ET0 = 1;
    TR0 = 1;
    PWMOUT =1;//initial output from PWM
}

//Adjust duty cycle 
void AdjustDutyCycle(unsigned char dc)
{
    unsigned int high, low;
    high = (PeriodCnt*dc)/100;
    low = PeriodCnt - high;
    high = 65536-high+12;
    low = 65536-low+12;
    HighRH = (unsigned char)(high>>8);
    HighRL = (unsigned char)high;
    LowRH = (unsigned char)(low>>8);
    LowRL = (unsigned char)low;
}

//Interrupt0 to assign different values during interrupt
void InterruptTimer0() interrupt 1
{
    if(PWMOUT ==1)
    {
        TH0 = LowRH;
        TL0 = LowRL;
        PWMOUT = 0;
    }
    else
    {
        TH0 = HighRH;
        TL0 = HighRL;
        PWMOUT = 1;
    }
}

//Interrupt1 change directions and call adjust duty cycle
void InterruptTImer1() interrupt 3
{
    //different level of brightness
    unsigned char code table[13] ={
        5,18,30,41,50,60,68,75,81,86,90,93,95
    }
    //direction of change dim->bright or bright->dim
    static bit dir = 0;
    static unsigned char index = 0;

    TH1 = T1RH;
    TL1 = T1RL;

    AdjustDutyCycle(table[index]);
    if(dir == 0)
    {
        index++;
        if(index>=12)
        {
            dir = 1;//change direction when all levels of brightness have been used
        }
    }
    else
    {
        index--;
        if(index==0)
        {
            dir = 0;
        }
    }
    


}