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
char temp = 0;


//Main function
int main(){


    WDTCTL = WDTPW | WDTHOLD;


    gpioConfig();
    uartConfig();
    wifiConfig();
    adcConfig();
    while(1){


        ADCCTL0 |= ADCENC | ADCSC;
        __bis_SR_register(GIE | LPM0_bits);


    }
    return 0;
}



//@TODO ADC Configure
void adcConfig(){


    //Configure ADC A4
    P1SEL1 |= BIT4;
    P1SEL0 |= BIT4;

    ADCCTL0 &= ~ADCSHT; //Clear ADCSHT
    ADCCTL0 |= ADCSHT_2;// Set sample and hold time to 16 cycles
    ADCCTL0 |= ADCON;   // turn ADC on

    ADCCTL1 |= ADCSSEL_SMCLK;
    ADCCTL1 |= ADCSHP;

    ADCMCTL0 |= ADCINCH_4; // select A4 as ADC
    ADCIE |= ADCIE0;

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
    //configr

}

//@TODO Send data to launchpad
void senData(val){

}

#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){

    temp = ADCMEM0; //Read ADC value
    __bic_SR_register_on_exit(LPM0_bits);//Wake up CPU
    senData(temp);//Send data to Launch pad



}
