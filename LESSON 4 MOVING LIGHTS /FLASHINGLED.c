#include<reg52.h>

sbit LED = P0^0;
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ADDR4 = P1^4;

void main(){

    unsgined int i = 0;//define i for 'for loop'
    unsinged char cnt = 0;//keep track on the time of drift
    //values from truth table to enable LES6 voltage
    ENLED = 0;
    ADDR3 = 1;
    ADDR2 = 1;
    ADDR1 = 0;

    //start loop
    while(1){
        //0x01 is 00000001 at fist with cnt=0
        //this means all LED light up but 1
        //use ~ to inverse P0 to get all LED off but only 1 light up
        //at cnt = 0 P0 = 11111110 which makes one LED on
        P0 = ~(0x01 << cnt)
        for(i=0;i<30000;i++);//use a empty 'for loop' to delay some time
        cnt++
        //use a for loop to reset cnt when cnt is greater than 8
        if(cnt>8){
            cnt = 0;
        }
        
    }
}