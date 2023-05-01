/*
* Group 5- Smart HVAC
* Takes input from LM334 and sends an integer to sensor
* LED and Fan handle is implemented here for demonstration purposes
* 3/27/23
* By Johnathan Schiede, Omar Bedewy, Edward Rosa
*/
#include <msp430.h>
#include "math.h"

//function prototyping
void adcConfig();
void gpioHandle();
void uartConfig();
void gpioConfig();
void conTemp(double val);
char temp = 100;


//Main function
int main(){


    WDTCTL = WDTPW | WDTHOLD;


    gpioConfig();
    uartConfig();
    adcConfig();

    PM5CTL0 &= ~LOCKLPM5;

    //Take UART out of Reset
    UCA1CTLW0 &= ~UCSWRST;

    //5. enable interrupts

    while(1){

        gpioHandle();
        ADCCTL0 |= ADCENC | ADCSC;//enable and start conversion

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

    ADCCTL1 |= ADCSSEL_2; //Select SMCLK
    ADCCTL1 |= ADCSHP;
    ADCCTL1 |= BIT1;      //repeated samples select
    ADCCTL2 &= ~BIT4;//clear LSB of ADCRES
    ADCCTL2 |= ADCRES_2;//set resolution to 8 bit

    ADCMCTL0 |= ADCINCH_4; // select A4 as ADC
    ADCIE |= ADCIE0;        //turn on ADC interrupt

}



void gpioHandle(){

    if (temp < 70){
        //Turn off fan and put RGB LED as Green
        P4OUT &= ~BIT4;
        P2OUT |= BIT5;
        P3OUT &= ~BIT0;
        P3OUT &= ~BIT2;
      }

    else if (temp >= 70 && temp< 75){
        //turn off fan put RGB LED as Yellow
          P4OUT |= BIT4;
          P2OUT |= BIT5;
          P3OUT &= ~BIT0;
          P3OUT &= ~BIT2;
     }

    else if (temp >= 75 && temp <90){
        //Put RGB LED As Red and turn fan off
        P4OUT &= ~BIT4;
        P2OUT &= ~BIT5;
        P3OUT |= BIT0;
        P3OUT |= BIT2;
     }

   else{
       //put RGB LED as Blue
        P4OUT |= BIT4;
        P2OUT &= ~BIT5;
        P3OUT &= ~BIT0;
        P3OUT &= ~BIT2;
     }


}
//@TODO UART Configure
void uartConfig(){

    //Put eUSCI_A4 into SW reset
    UCA0CTLW0 |= UCSWRST;

    //Configure eUSCI_A4
    UCA0CTLW0 |= UCSSEL__SMCLK;

    UCA0BRW = 8;
    UCA0MCTLW |= 0xD600;

    //configure P1.7 as Tx
    P1SEL1 &= ~BIT7;
    P1SEL0 |= BIT7;


    //configure P1.6 as Rx
    P1SEL1 &= ~BIT6;
    P1SEL0 |= BIT6;


}



void gpioConfig(){

    //Blue LED
    P4OUT &= ~BIT4;
    P4DIR |= BIT4;
    //Green LED
    P2OUT &= ~BIT5;
    P2DIR |= BIT5;
    //Red LED
    P3OUT &= ~BIT0;
    P3DIR |= BIT0;

    //Fan
    P3OUT &= ~BIT2;
    P3DIR |= BIT2;

}

//@TODO Send data to launchpad
void conTemp(double val){
   temp = ((val/.010)-273.15)*1.8 +32;// voltage to fahrenheit conversion
    //casting value to a char
}

#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){

    double adcVal = ADCMEM0; //Read ADC value
    adcVal = adcVal*.000805;
    __bic_SR_register_on_exit(LPM0_bits);//Wake up CPU
    conTemp(adcVal);
    UCA0IE |= UCTXCPTIE;//enable transmission interrupt request
    UCA0IFG &= ~UCTXCPTIFG;// clear UCTXCPTIFG flag
    UCA0TXBUF = temp;

}
#pragma vector = EUSCI_A1_VECTOR// UART Transmission Interrupt Vector
__interrupt void ISR_EUSCI_A1(void)
{
    UCA0IE &= ~UCTXCPTIE;//clearing interrupt enable an interrupt flag
    UCA0IFG &= ~UCTXCPTIFG;//since there is only one character that needs to be shifted out this flag can be immediately cleared
}


