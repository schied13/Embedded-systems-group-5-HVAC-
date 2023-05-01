/**********************************
* Smart HVAC System Launchpad Program
*
* Takes serial input from UART, lights RGB LED depending on the Temperature
* Displays temperature on LCD screen
*
* Author: Johnathan Schiede, Eddie Rosa, Omar Bedewy
* Version: 1.0
*
* Peripherals Used:
* ADC12 - Channel 1 (Pin 1.1)
* GPIO -
* TimerB0 - UP Mode, CCR1 for PWM Control, controls Pin 6.0
*/
#include <msp430.h>

//Function Prototyping
void gpioConfig();
void fanHandle();
void uartConfig();
void gpioHandle();


char temperature = 1;

int main(){

    WDTCTL = WDTPW | WDTHOLD;


    gpioConfig();
    uartConfig();


    PM5CTL0 &= ~LOCKLPM5;

    //Take UART out of Reset
    UCA1CTLW0 &= ~UCSWRST;
    //enable RX interrupt
    UCA1IE |= UCRXIE;
    __enable_interrupt();


    while(1){

        gpioHandle();

    }

    return 0;
}



//@TODO GPIO Config
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






//@TODO UART Config
void uartConfig(){
    //Put eUSCI_A4 into SW reset
    UCA1CTLW0 |= UCSWRST;

    //Configure eUSCI_A4
    UCA1CTLW0 |= UCSSEL__SMCLK;

    UCA1BRW = 8;
    UCA1MCTLW |= 0xD600;

    //configure P1.7 as Tx
    P1SEL1 &= ~BIT7;
    P1SEL0 |= BIT7;


    //configure P1.6 as Rx
    P1SEL1 &= ~BIT6;
    P1SEL0 |= BIT6;

}



//@TODO configure LED using PWM
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

#pragma vector = EUSCI_A1_VECTOR
__interrupt void EUSCI_A1_RX_ISR(void)
{
    temperature = UCA1RXBUF;//set received message as the temperature

}
