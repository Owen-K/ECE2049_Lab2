/*
 * timer.c
 *
 *  Created on: Sep 21, 2022
 *      Author: okrause
 */

#include <msp430.h>
#include "timer.h"

//sets clock variable
unsigned long int timer_cnt = 0;

void setupTimerA2(){
    TA2CTL = TASSEL_2 + MC_1 + ID_0;
    TA2CCR0 = 5242; //set to 5ms resolution
    TA2CCTL0 = CCIE;
}

#pragma vector=TIMER2_A0_VECTOR
__interrupt void TIMER2_A0_ISR (void) {
    timer_cnt++;
}

