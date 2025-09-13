#include "actuator_drivers.h"

// LED Timer configuration
#define LED_TOGGLE_PERIOD_MS 500
#define F_CPU 16000000UL
// Using prescaler of 1024
#define LED_OCR_VALUE ((F_CPU / 1024) * LED_TOGGLE_PERIOD_MS / 1000)

void led_init(void) {
    DDRB |= (1 << PB0); // Set LED pin as output
    // Configure Timer0 in CTC mode
    TCCR0 |= (1 << WGM01); // CTC Mode [21]
    TCNT0 = 0; // Reset timer counter
    OCR0 = (uint8_t)LED_OCR_VALUE; // Set compare value
    TIMSK |= (1 << OCIE0); // Enable Timer0 Compare Match Interrupt [17]
    TCCR0 |= (1 << CS02) | (1 << CS00); // Set prescaler to 1024 [22]
    sei(); // Enable global interrupts
}

void led_toggle(void) {
    PORTB ^= (1 << PB0); // Toggle the LED pin
}

void buzzer_init(void) {
    DDRB |= (1 << PB3); // Set OC0 (PB3) pin as output for PWM [19]
    // Configure Timer0 for Fast PWM mode, non-inverted output
    TCCR0 |= (1 << WGM00) | (1 << WGM01); // Fast PWM Mode
    TCCR0 |= (1 << COM01); // Clear OC0 on compare match, set at TOP [19]
    // Start timer with prescaler
    TCCR0 |= (1 << CS00); // No prescaling, for a high frequency tone
    // Set initial duty cycle to 0
    OCR0 = 0;
}

void buzzer_on(void) {
    // Set duty cycle to 50%
    OCR0 = (uint8_t)(255 / 2); // Set compare value for 50% duty cycle
}

void buzzer_off(void) {
    // Set duty cycle to 0%
    OCR0 = 0;
}

ISR(TIMER0_COMP_vect) {
    led_toggle(); // ISR to toggle the LED
}
