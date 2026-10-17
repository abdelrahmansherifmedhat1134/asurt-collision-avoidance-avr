/*
 * HCSR04.c
 *
 *  Created on: Sep 12, 2025 (PlatformIO port: Oct 17, 2026)
 *      Author: Abdelrahman Sherif Medhat
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <util/delay.h>
#include "HCSR04.h"
#include "HCSR04_cfg.h"

/* Timer1 at F_CPU/64: 4 us per tick at 16 MHz, so the 40 ms timeout is 10000 ticks (no overflow) */
#define HCSR04_TIMER1_CLOCK_BITS   ((1 << CS11) | (1 << CS10))
#define HCSR04_US_PER_TICK         (64UL * 1000000UL / F_CPU)
#define HCSR04_TIMEOUT_TICKS       ((uint16_t)(HCSR04_TIMEOUT_MS * 1000UL / HCSR04_US_PER_TICK))

static volatile uint16_t HCSR04_u16RiseTick;
static volatile uint16_t HCSR04_u16FallTick;
static volatile uint8_t  HCSR04_u8WaitingForFall;
static volatile uint8_t  HCSR04_u8EchoDone;

void HCSR04_voidInit(void){
	HCSR04_TRIG_PORT &= ~(1 << HCSR04_TRIG_PIN);
	HCSR04_TRIG_DDR  |=  (1 << HCSR04_TRIG_PIN);
	DDRD   &= ~(1 << PD6);        /* ECHO -> ICP1 */
	TCCR1A  = 0;                  /* normal mode */
	TCCR1B  = 0;                  /* stopped */
	TIMSK  |= (1 << TICIE1);
}

uint16_t HCSR04_u16TicksToCm(uint16_t Copy_u16Ticks){
	/* distance [cm] = echo width [us] / 58, rounded to the nearest cm */
	return (uint16_t)(((uint32_t)Copy_u16Ticks * HCSR04_US_PER_TICK + 29UL) / 58UL);
}

uint16_t HCSR04_u16MeasureCm(void){
	uint16_t Local_u16Now;

	HCSR04_u8WaitingForFall = 0;
	HCSR04_u8EchoDone       = 0;
	TCCR1B = 0;
	TCNT1  = 0;
	TIFR   = (1 << ICF1);
	TCCR1B = (1 << ICNC1) | (1 << ICES1) | HCSR04_TIMER1_CLOCK_BITS;   /* capture the rising edge first */

	HCSR04_TRIG_PORT |=  (1 << HCSR04_TRIG_PIN);
	_delay_us(10);
	HCSR04_TRIG_PORT &= ~(1 << HCSR04_TRIG_PIN);

	do{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE){ Local_u16Now = TCNT1; }
	}while(!HCSR04_u8EchoDone && Local_u16Now < HCSR04_TIMEOUT_TICKS);

	TCCR1B = 0;
	if(!HCSR04_u8EchoDone){
		return HCSR04_NO_ECHO;
	}
	return HCSR04_u16TicksToCm(HCSR04_u16FallTick - HCSR04_u16RiseTick);
}

ISR(TIMER1_CAPT_vect){
	if(!HCSR04_u8WaitingForFall){
		HCSR04_u16RiseTick = ICR1;
		TCCR1B &= ~(1 << ICES1);  /* next capture on the falling edge */
		TIFR    =  (1 << ICF1);   /* changing the edge can set ICF1 */
		HCSR04_u8WaitingForFall = 1;
	}else{
		HCSR04_u16FallTick = ICR1;
		HCSR04_u8EchoDone  = 1;
	}
}
