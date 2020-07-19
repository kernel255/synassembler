#include <msp430.h> 


/**
 * main.c
 */


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	P1DIR |= BIT0 |BIT6;
	//P1OUT &= ~BIT0;
	P1OUT = BIT0;
	P1OUT = ~BIT6;

	P1IE |= BIT3;
	P1IFG &= ~BIT3;


	__bis_SR_register(GIE);

	while(1)
	{
	}
	
	return 0;
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    P1OUT ^= BIT0;  // Toggle P1.0
    P1IFG &= ~BIT3; // P1.3 interrupt flag cleared
}
