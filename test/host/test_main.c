/* Host unit tests: drivers against mocked ATmega32 registers, and the warning logic against
 * the FSUK26 task: when distance <= 50 cm, toggle the LED every 500 ms and toggle the buzzer
 * every 10 x distance ms (e.g. 45 cm -> 450 ms on, 450 ms off). */
#include <stdio.h>
#include <avr/io.h>
#include "mock_regs.h"
#include "USART/USART.h"
#include "HCSR04/HCSR04.h"
#include "BUZZER/BUZZER.h"
#include "LED/LED.h"
#include "SYSTICK/SYSTICK.h"
#include "COLLISION/COLLISION.h"

void USART_RXC_vect(void);
void TIMER2_COMP_vect(void);

static int g_checks, g_failures;

#define CHECK(cond) do{ g_checks++; if(!(cond)){ g_failures++; \
	printf("  FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); } }while(0)
#define CHECK_EQ(a, b) do{ long _a = (long)(a), _b = (long)(b); g_checks++; if(_a != _b){ g_failures++; \
	printf("  FAIL %s:%d: %s == %ld, expected %ld\n", __FILE__, __LINE__, #a, _a, _b); } }while(0)

/* echo width in Timer1 ticks (4 us) for a distance in cm */
static uint16_t echo_ticks(uint16_t cm){ return (uint16_t)(cm * 58u / 4u); }

static void test_usart(void){
	puts("USART: 9600 8N1 at 16 MHz, TX, RX interrupt");
	mock_reset_registers();
	USART_voidInit();
	CHECK_EQ(UBRRH, 0);
	CHECK_EQ(UBRRL, 103);
	CHECK_EQ(UCSRB, (1 << RXEN) | (1 << TXEN));
	CHECK_EQ(UCSRC, (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0));
	USART_voidEnableRxInterrupt();
	CHECK(UCSRB & (1 << RXCIE));

	USART_voidSendByte(45);
	CHECK_EQ(UDR, 45);

	CHECK_EQ(USART_u8IsByteReceived(), 0);
	UDR = 37; USART_RXC_vect();
	CHECK_EQ(USART_u8IsByteReceived(), 1);
	CHECK_EQ(USART_u8ReadByte(), 37);
	CHECK_EQ(USART_u8IsByteReceived(), 0);
}

static void test_hcsr04(void){
	puts("HCSR04: pins, Timer1 input capture, distance, timeout");
	mock_reset_registers();
	HCSR04_voidInit();
	CHECK(DDRA & (1 << PA6));                /* TRIG output */
	CHECK(!(DDRD & (1 << PD6)));             /* ECHO on ICP1 input */
	CHECK(TIMSK & (1 << TICIE1));
	CHECK_EQ(TCCR1B, 0);                     /* stopped until a measurement */

	CHECK_EQ(HCSR04_u16TicksToCm(echo_ticks(50)), 50);
	CHECK_EQ(HCSR04_u16TicksToCm(echo_ticks(10)), 10);
	CHECK_EQ(HCSR04_u16TicksToCm(echo_ticks(400)), 400);

	mock_set_echo(120, 120 + echo_ticks(45));
	CHECK_EQ(HCSR04_u16MeasureCm(), 45);
	CHECK_EQ(TCCR1B, 0);                     /* timer stopped again */
	CHECK(!(PORTA & (1 << PA6)));            /* TRIG back low */

	mock_set_echo(50, 50 + echo_ticks(50));  /* boundary distance */
	CHECK_EQ(HCSR04_u16MeasureCm(), 50);

	mock_set_echo(80, 80 + echo_ticks(300)); /* far obstacle, > 255 cm */
	CHECK_EQ(HCSR04_u16MeasureCm(), 300);

	mock_set_echo(MOCK_NO_EDGE, MOCK_NO_EDGE);  /* sensor disconnected: no echo at all */
	CHECK_EQ(HCSR04_u16MeasureCm(), HCSR04_NO_ECHO);
	CHECK_EQ(mock_timer1_ticks(), 10000);    /* gave up after exactly 40 ms */

	mock_set_echo(100, 15000);               /* echo longer than the timeout */
	CHECK_EQ(HCSR04_u16MeasureCm(), HCSR04_NO_ECHO);

	mock_set_echo(200, 200 + echo_ticks(20)); /* recovers on the next measurement */
	CHECK_EQ(HCSR04_u16MeasureCm(), 20);
}

static void test_actuator_timers(void){
	puts("Actuator: LED, buzzer and system tick do not share a timer");
	mock_reset_registers();
	LED_voidInit();
	BUZZER_voidInit();
	SYSTICK_voidInit();

	CHECK_EQ(TCCR0, (1 << WGM01) | (1 << CS01) | (1 << CS00));  /* Timer0: CTC tone, /64 */
	CHECK_EQ(OCR0, 124);
	CHECK_EQ(16000000L / (2L * 64L * (1L + OCR0)), 1000);        /* 1 kHz tone */
	CHECK_EQ(TCCR2, (1 << WGM21) | (1 << CS22));                 /* Timer2: CTC, /64 */
	CHECK_EQ(OCR2, 249);                                         /* 250 kHz / 250 = 1 kHz tick */
	CHECK(TIMSK & (1 << OCIE2));
	CHECK(!(TIMSK & ((1 << OCIE0) | (1 << TOIE0))));             /* no Timer0 interrupt */
	CHECK((DDRB & ((1 << PB0) | (1 << PB3))) == ((1 << PB0) | (1 << PB3)));

	BUZZER_voidOn();
	CHECK(TCCR0 & (1 << COM00));
	CHECK_EQ(TCCR0 & ~(1 << COM00), (1 << WGM01) | (1 << CS01) | (1 << CS00));
	BUZZER_voidOff();
	CHECK(!(TCCR0 & (1 << COM00)));
	CHECK(!(PORTB & (1 << PB3)));

	LED_voidOn();
	CHECK(PORTB & (1 << PB0));
	CHECK_EQ(TCCR0, (1 << WGM01) | (1 << CS01) | (1 << CS00));  /* LED leaves Timer0 alone */
	LED_voidOff();
	CHECK(!(PORTB & (1 << PB0)));

	for(int i = 0; i < 1500; i++){ TIMER2_COMP_vect(); }
	CHECK_EQ(SYSTICK_u32Millis(), 1500);
}

/* Runs the warning logic in 1 ms steps; the sensor sends `cm` every 100 ms from t0 to t1.
 * Records the times at which the LED and buzzer outputs change. */
typedef struct{ uint32_t led[64]; int nled; uint32_t buz[64]; int nbuz; uint8_t led_on, buz_on; }trace_t;

static void run(trace_t *t, uint32_t from, uint32_t to, int cm, uint32_t send_until){
	for(uint32_t now = from; now < to; now++){
		if(cm >= 0 && now <= send_until && now % 100 == 0){ COLLISION_voidOnDistance((uint8_t)cm, now); }
		COLLISION_tstrOutputs o = COLLISION_strUpdate(now);
		if(o.u8LedOn != t->led_on && t->nled < 64)    { t->led[t->nled++] = now; t->led_on = o.u8LedOn; }
		if(o.u8BuzzerOn != t->buz_on && t->nbuz < 64) { t->buz[t->nbuz++] = now; t->buz_on = o.u8BuzzerOn; }
	}
}

static void test_collision(void){
	trace_t t;

	puts("COLLISION: no warning above 50 cm");
	COLLISION_voidInit(); t = (trace_t){0};
	run(&t, 0, 3000, 51, 3000);
	CHECK_EQ(t.nled, 0); CHECK_EQ(t.nbuz, 0);

	puts("COLLISION: 45 cm -> LED toggles every 500 ms, buzzer 450 ms on / 450 ms off");
	COLLISION_voidInit(); t = (trace_t){0};
	run(&t, 0, 2001, 45, 2001);
	CHECK_EQ(t.nled, 5);
	CHECK_EQ(t.led[0], 0);   CHECK_EQ(t.led[1], 500);  CHECK_EQ(t.led[2], 1000);
	CHECK_EQ(t.led[3], 1500); CHECK_EQ(t.led[4], 2000);
	CHECK_EQ(t.nbuz, 5);
	CHECK_EQ(t.buz[0], 0);   CHECK_EQ(t.buz[1], 450);  CHECK_EQ(t.buz[2], 900);
	CHECK_EQ(t.buz[3], 1350); CHECK_EQ(t.buz[4], 1800);

	puts("COLLISION: exactly 50 cm warns (<= 50)");
	COLLISION_voidInit(); t = (trace_t){0};
	run(&t, 0, 1001, 50, 1001);
	CHECK_EQ(t.buz[0], 0); CHECK_EQ(t.buz[1], 500); CHECK_EQ(t.buz[2], 1000);

	puts("COLLISION: distance change updates the buzzer period");
	COLLISION_voidInit(); t = (trace_t){0};
	run(&t, 0, 1000, 45, 999);               /* toggles at 0, 450, 900 */
	run(&t, 1000, 1601, 20, 1601);           /* 200 ms from the last toggle: 1100, 1300, 1500 */
	CHECK_EQ(t.nbuz, 6);
	CHECK_EQ(t.buz[2], 900); CHECK_EQ(t.buz[3], 1100); CHECK_EQ(t.buz[4], 1300); CHECK_EQ(t.buz[5], 1500);

	puts("COLLISION: obstacle leaves -> LED and buzzer off; returns -> pattern restarts");
	COLLISION_voidInit(); t = (trace_t){0};
	run(&t, 0, 700, 30, 699);                /* buzzer on at 0, off 300, on 600 */
	run(&t, 700, 1000, 80, 999);
	CHECK_EQ(t.led_on, 0); CHECK_EQ(t.buz_on, 0);
	CHECK_EQ(t.buz[t.nbuz - 1], 700);        /* switched off as soon as 80 cm arrives */
	run(&t, 1000, 1001, 30, 1001);
	CHECK_EQ(t.led_on, 1); CHECK_EQ(t.buz_on, 1);

	puts("COLLISION: no readings for 500 ms (link lost) -> warning stops");
	COLLISION_voidInit(); t = (trace_t){0};
	run(&t, 0, 1000, 20, 0);                 /* one reading at t = 0 only; buzzer on 0, off 200, on 400 */
	CHECK_EQ(t.led_on, 0); CHECK_EQ(t.buz_on, 0);
	CHECK_EQ(t.buz[t.nbuz - 1], 500);        /* forced off when the reading is 500 ms old */

	puts("COLLISION: 0 cm keeps the buzzer on");
	COLLISION_voidInit(); t = (trace_t){0};
	run(&t, 0, 1000, 0, 1000);
	CHECK_EQ(t.nbuz, 1); CHECK_EQ(t.buz_on, 1);
}

int main(void){
	test_usart();
	test_hcsr04();
	test_actuator_timers();
	test_collision();
	printf("\n%d checks, %d failed\n", g_checks, g_failures);
	return g_failures ? 1 : 0;
}
