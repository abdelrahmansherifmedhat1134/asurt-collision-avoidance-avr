
#include "hcsr04_driver.h"
#include <util/delay.h>

// Global flags and variables for the Input Capture Unit
volatile uint8_t g_echo_state = 0;
volatile uint16_t g_timer_start = 0;
volatile uint16_t g_timer_end = 0;
volatile uint8_t g_measurement_ready = 0;

void hcsr04_init(void) {
    // Configure the Trigger pin as output and Echo pin as input
    DDRA |= (1 << PA6); // TRIG pin
    DDRA &= ~(1 << PA5); // ECHO pin

    // Configure Timer/Counter1 for Input Capture
    TCCR1B = 0; // Stop the timer first
    TCNT1 = 0;  // Reset counter
    TIFR = (1 << ICF1); // Clear Input Capture Flag

    // Enable the Input Capture Interrupt
    TIMSK |= (1 << TICIE1);
    sei(); // Enable global interrupts
}

void hcsr04_trigger_pulse(void) {
    g_measurement_ready = 0;
    PORTA |= (1 << PA6); // Set TRIG pin high
    _delay_us(10);       // Wait for 10us [9]
    PORTA &= ~(1 << PA6); // Set TRIG pin low
}

uint8_t hcsr04_is_measurement_ready(void) {
    return g_measurement_ready;
}

void hcsr04_reset_measurement_flag(void) {
    g_measurement_ready = 0;
}

uint16_t hcsr04_read_distance_cm(void) {
    uint32_t pulse_time_us;
    uint16_t distance_cm;

    if (g_timer_end > g_timer_start) {
        pulse_time_us = (g_timer_end - g_timer_start) / (F_CPU / 1000000UL);
    } else {
        // Handle timer overflow
        pulse_time_us = (uint32_t)(65535 - g_timer_start + g_timer_end) / (F_CPU / 1000000UL);
    }

    // Distance = time_us / 58.3 [8]
    distance_cm = (uint16_t) (pulse_time_us / 58.3);

    return distance_cm;
}

ISR(TIMER1_CAPT_vect) {
    if (g_echo_state == 0) {
        // Rising edge detected
        g_timer_start = ICR1;
        g_echo_state = 1;
        // Change polarity to falling edge
        TCCR1B &= ~(1 << ICES1);
    } else {
        // Falling edge detected
        g_timer_end = ICR1;
        g_echo_state = 0;
        g_measurement_ready = 1;
        // Stop timer and reset
        TCCR1B = 0;
        // Change polarity back to rising edge
        TCCR1B |= (1 << ICES1);
    }
}

