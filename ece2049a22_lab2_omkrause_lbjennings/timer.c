/*
 * timer.c
 *
 *  Created on: Sep 21, 2022
 *      Author: okrause
 */

#include "timer.h"


void setupTimerA2(){
    TA2CTL = TASSEL_2 + MC_1 + ID_0;
    TA2CCR0 = 5242;
    TA2CCTL0 = CCIE;
}

#pragma vector = TIMER2_A0_VECTOR
_interupt void Timer_A2_ISR(void) {

}
