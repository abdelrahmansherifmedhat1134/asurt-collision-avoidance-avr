/*
 * BUZZER.c
 *
 *  Created on: Sep 12, 2025 (PlatformIO port: Oct 17, 2026)
 *      Author: Abdelrahman Sherif Medhat
 */
#include <avr/io.h>
#include "BUZZER.h"
#include "BUZZER_cfg.h"

void BUZZER_voidInit(void){
	PORTB &= ~(1 << PB3);
	DDRB  |=  (1 << PB3);
#if BUZZER_USE_TONE
	OCR0  = BUZZER_TONE_OCR0;
	TCCR0 = (1 << WGM01) | (1 << CS01) | (1 << CS00);   /* CTC, F_CPU/64, OC0 disconnected */
#endif
}

void BUZZER_voidOn(void){
#if BUZZER_USE_TONE
	TCCR0 |= (1 << COM00);                              /* toggle OC0 on every compare match */
#else
	PORTB |= (1 << PB3);
#endif
}

void BUZZER_voidOff(void){
#if BUZZER_USE_TONE
	TCCR0 &= ~(1 << COM00);                             /* PB3 returns to its PORTB value */
#endif
	PORTB &= ~(1 << PB3);
}
