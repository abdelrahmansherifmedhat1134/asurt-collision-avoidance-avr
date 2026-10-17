/* Register storage for the host tests, plus a Timer1 / HC-SR04 echo simulation:
 * every read of TCNT1 advances the running timer by one tick, and the echo edges set by
 * the test are captured into ICR1 (calling the capture ISR) when the matching edge is armed. */
#include <avr/io.h>
#include "mock_regs.h"

volatile uint8_t  DDRA, PORTA, DDRB, PORTB, DDRD, PORTD;
volatile uint8_t  TCCR0, OCR0, TCCR1A, TCCR1B, TCCR2, OCR2, TCNT2, TIMSK, TIFR;
volatile uint16_t ICR1;
volatile uint8_t  UBRRH, UBRRL, UCSRA, UCSRB, UCSRC, UDR;

static volatile uint16_t mock_u16Tcnt1;
static uint16_t          mock_u16EchoRise = MOCK_NO_EDGE;
static uint16_t          mock_u16EchoFall = MOCK_NO_EDGE;

void TIMER1_CAPT_vect(void);

void mock_reset_registers(void){
	DDRA = PORTA = DDRB = PORTB = DDRD = PORTD = 0;
	TCCR0 = OCR0 = TCCR1A = TCCR1B = TCCR2 = OCR2 = TCNT2 = TIMSK = TIFR = 0;
	ICR1 = 0;
	UBRRH = UBRRL = UCSRB = UCSRC = UDR = 0;
	UCSRA = (1 << UDRE);                 /* transmit buffer always empty */
	mock_u16Tcnt1 = 0;
	mock_u16EchoRise = mock_u16EchoFall = MOCK_NO_EDGE;
}

void mock_set_echo(uint16_t rise_tick, uint16_t fall_tick){
	mock_u16EchoRise = rise_tick;
	mock_u16EchoFall = fall_tick;
}

uint16_t mock_timer1_ticks(void){
	return mock_u16Tcnt1;
}

volatile uint16_t *mock_tcnt1(void){
	if(TCCR1B & 0x07){                   /* clock selected: timer running */
		mock_u16Tcnt1++;
		if(TIMSK & (1 << TICIE1)){
			uint8_t rising = (TCCR1B >> ICES1) & 1;
			if((rising && mock_u16Tcnt1 == mock_u16EchoRise) || (!rising && mock_u16Tcnt1 == mock_u16EchoFall)){
				ICR1 = mock_u16Tcnt1;
				TIMER1_CAPT_vect();
			}
		}
	}
	return &mock_u16Tcnt1;
}
