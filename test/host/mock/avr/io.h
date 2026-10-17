/* Host-test stand-in for <avr/io.h>: ATmega32 registers as plain variables.
 * TCNT1 is a function call so the test can simulate Timer1 counting and the HC-SR04 echo. */
#ifndef MOCK_AVR_IO_H_
#define MOCK_AVR_IO_H_

#include <stdint.h>

extern volatile uint8_t  DDRA, PORTA, DDRB, PORTB, DDRD, PORTD;
extern volatile uint8_t  TCCR0, OCR0, TCCR1A, TCCR1B, TCCR2, OCR2, TCNT2, TIMSK, TIFR;
extern volatile uint16_t ICR1;
extern volatile uint8_t  UBRRH, UBRRL, UCSRA, UCSRB, UCSRC, UDR;

volatile uint16_t *mock_tcnt1(void);
#define TCNT1 (*mock_tcnt1())

/* port pins */
#define PA6 6
#define PB0 0
#define PB3 3
#define PD6 6

/* Timer0 */
#define WGM00 6
#define COM01 5
#define COM00 4
#define WGM01 3
#define CS02  2
#define CS01  1
#define CS00  0
/* Timer1 */
#define ICNC1 7
#define ICES1 6
#define CS12  2
#define CS11  1
#define CS10  0
/* Timer2 */
#define WGM21 3
#define CS22  2
#define CS21  1
#define CS20  0
/* TIMSK / TIFR */
#define OCIE2  7
#define TICIE1 5
#define OCIE0  1
#define TOIE0  0
#define ICF1   5
/* USART */
#define RXC   7
#define UDRE  5
#define RXCIE 7
#define RXEN  4
#define TXEN  3
#define URSEL 7
#define UCSZ1 2
#define UCSZ0 1

#endif /* MOCK_AVR_IO_H_ */
