/*
 * timer.c
 *
 *  Created on: Sep 21, 2022
 *      Author: okrause
 */

#include <msp430.h>
#include "timer.h"


unsigned long int TimerCount = 0;

void setupTimerA2(){
    TA2CTL = TASSEL_2 + MC_1 + ID_0;
    TA2CCR0 = 5242;
    TA2CCTL0 = CCIE;
}

<<<<<<< HEAD
#pragma vector=TIMER2_A0_VECTOR
__interrupt void TIMER2_A0_ISR (void) {
    TimerCount++;
}
=======
/*#pragma vector = TIMER2_A0_VECTOR
_interupt void Timer_A2_ISR(void) {
    timer++;
}*/
>>>>>>> 253aebeef1e9982395dde9a0ad7a0234d108317a
