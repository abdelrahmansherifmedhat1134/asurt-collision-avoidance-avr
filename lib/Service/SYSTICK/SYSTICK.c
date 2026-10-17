/*
 * SYSTICK.c
 *
 *  Created on: Sep 12, 2025 (PlatformIO port: Oct 17, 2026)
 *      Author: Abdelrahman Sherif Medhat
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "SYSTICK.h"

/* Timer2 at F_CPU/64 = 250 kHz: 250 counts = 1 ms */
#define SYSTICK_OCR2_VALUE   ((uint8_t)(F_CPU / 64UL / 1000UL - 1UL))

static volatile uint32_t SYSTICK_u32Ms;

void SYSTICK_voidInit(void){
	TCNT2  = 0;
	OCR2   = SYSTICK_OCR2_VALUE;
	TCCR2  = (1 << WGM21) | (1 << CS22);   /* CTC, F_CPU/64 */
	TIMSK |= (1 << OCIE2);
}

uint32_t SYSTICK_u32Millis(void){
	uint32_t Local_u32Ms;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){ Local_u32Ms = SYSTICK_u32Ms; }
	return Local_u32Ms;
}

ISR(TIMER2_COMP_vect){
	SYSTICK_u32Ms++;
}
