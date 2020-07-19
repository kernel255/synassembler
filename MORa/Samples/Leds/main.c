#include <msp430.h>

// Blue   LED 0x08
// White  LED 0x20
// Orange LED 0x10

#define DELAY 20000



/**
 * blink.c
 */
void main(void)
{
    unsigned int blueVals[4];

    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer
    P1DIR |= 0xFF;                  // configure P1.0 as output
    P2DIR |= 0x3F;

    volatile unsigned int i;        // volatile to prevent optimization
    volatile unsigned int j;




    blueVals[0] = DELAY;
    blueVals[1] = DELAY*2;
    blueVals[2] = DELAY*3;
    blueVals[3] = DELAY*4;

    __bis_SR_register(GIE);

    while(1)
    {
        int len;

        for(i=DELAY; i>0; i--) {
        }
        P1OUT ^= 0x40;            // toggle P1.0

        /*
        for(j=0;j<4;j++) {
            len = blueVals[j];
            for(i=len; i>0; i--) {
                P2OUT ^= 0x10;
            }
        }
        for(i=DELAY; i>0; i--) {
            P2OUT = 0x08;
        }
        for(i=DELAY; i>0; i--) {
            P2OUT = 0x20;
        }
        */
    }
}
