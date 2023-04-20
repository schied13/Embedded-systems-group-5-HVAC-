/**********************************
* Smart HVAC System Launchpad Program
*
* Takes input Voltage from LM334(P1.1) into ADC A1
* Configures WiFi Module to TX mode
* Sends Data from UART to WiFi Module
*
* Author: Johnathan Schiede, Eddie Rosa, Omar Bedewy
* Version: 1.0
*
* Peripherals Used:
* ADC12 - Channel 1 (Pin 1.1)
* GPIO - Pin
* TimerB0 - UP Mode, CCR1 for PWM Control, controls Pin 6.0
*/


//function prototyping
void adcConfig();
void wifiConfig();
void uartConfig();
void gpioConfig();



//Main function
int main(){

    while(1){

        //@TODO Temperature Conversion From ADC Value


    }
    return 0;
}



//@TODO ADC Configure
void adcConfig(){


    //Configure ADC A4
    P1SEL1 |= BIT4;
    P1SEL0 |= BIT4;

}



//@TODO WIFI Configure
void wifiConfig(){
    //I dont even have an idea
    //check datasheet
}



//@TODO UART Configure
void uartConfig(){

    //Put eUSCI_A4 into SW reset
    UCA1CTLW0 |= UCSWRST;

    //Configure eUSCI_A4
    UCA1CTLW0 |= UCSSEL__SMCLK;

    UCA1BRW = 8;
    UCA1MCTLW |= 0xD600;

    //configure P1.7 as Tx
    P1SEL0 &= ~BIT7;
    P1SEL1 |= BIT7;


    //configure P1.6 as Rx
    P1SEL0 &= ~BIT6;
    P1SEL1 |= BIT6;


}



//@TODO GPIO Configure
void gpioConfig(){


}


