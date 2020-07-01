#include<reg52.h>

//initial values for display
unsigned char code LedChar[] = {
    0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E
};

//there are 6 display lights and initially all 0 turned off
unsigned char LedBuff[6]={
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
}

void main()
{
    //since char can only go to 255, we need int here to count more
    unsigned int cnt = 0;
    unsigned long sec = 0;
    unsigned char i = 0;
    //get pins for 74HC138 
    sbit ENLED = P1^4;//enable P1
    sbit ADDR0 = P1^0;//change different display light
    sbit ADDR1 = P1^1;//change ~
    sbit ADDR2 = P1^2;//change ~
    sbit ADDR3 = P1^3;//enable P1

    TMOD = 0x01;
    TH0 = 0xFC;
    TL0 = 0x67;
    TR0 = 1;

    while(1)
    {
        if(TF0 ==1 )
        {
            TF0 = 0;
            TH0 = 0xFC;
            TL0 = 0x67;
            cnt++;
            
            if(cnt>=1000)
            {
                cnt = 0;
                sec++;
                LedBuff[0] = LedChar[sec%10];
                LedBuff[1] = LedChar[sec/10%10];
                LedBuff[2] = LedChar[sec/100%10];
                LedBuff[3] = LedChar[sec/1000%10];
                LedBuff[4] = LedChar[sec/10000%10];
                LedBuff[5] = LedChar[sec/100000%10];
            }

            if(i==0)
            {
                ADDR2 = 0;
                ADDR1 = 0;
                ADDR0 = 0;
                i++;
                P0 = LedBuff[0];
            }else if(i==1){
                ADDR2 = 0;
                ADDR1 = 0;
                ADDR0 = 1;
                i++;
                P0 = LedBuff[1];
            }else if(i==2){
                ADDR2 = 0;
                ADDR1 = 1;
                ADDR0 = 0;
                i++;
                P0 = LedBuff[2];
            }else if(i==3){
                ADDR2 = 0;
                ADDR1 = 1;
                ADDR0 = 1;
                i++;
                P0 = LedBuff[3];
            }else if(i==4){
                ADDR2 = 1;
                ADDR1 = 0;
                ADDR0 = 0;
                i++;
                P0 = LedBuff[4];
            }else if(i==5){
                ADDR2 = 1;
                ADDR1 = 0;
                ADDR0 = 1;
                i++;
                P0 = LedBuff[5];
            }
            // this if loop can be rewrite by using switch
            //case 0:ADDR2 = 0;ADDR1=0;ADDR0=0;i++;P0=LedBuff[0];break;
            //default;break;
        }
    }
}