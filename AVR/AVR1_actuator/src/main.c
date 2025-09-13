#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "C:\temp\AVR1_actuator\services\uart_rx_service.h"
#include "C:\temp\AVR1_actuator\drivers\actuator_drivers.h"
#include "C:\temp\AVR1_actuator\services\collision_service.h"

// A simple software counter to manage non-blocking delays
static uint32_t g_millis_counter = 0;

void timer_init_for_millis(void) {
    TCCR2 |= (1 << WGM21); // CTC Mode
    TCNT2 = 0;
    // For 16MHz clock, 1024 prescaler, OCR2 value of 155 gives 1ms tick
    OCR2 = 155;
    TIMSK |= (1 << OCIE2); // Enable Timer2 Compare Match Interrupt
}

ISR(TIMER2_COMP_vect) {
    g_millis_counter++;
}

uint32_t millis(void) {
    uint32_t ms;
    cli(); // Disable interrupts temporarily
    ms = g_millis_counter;
    sei(); // Re-enable interrupts
    return ms;
}

int main(void) {
    // Initialize the systems
    uart_init();
    led_init();
    buzzer_init();
    timer_init_for_millis();

    // Main application loop
    while (1) {
        // Check for new data from UART
        if (uart_has_new_data()) {
            uint8_t distance = uart_read_data();
            collision_service_update(distance);
        }
    }
    return 0;
}

