#include "uart_rx_service.h"

// Global buffer for received data
volatile uint8_t g_received_data = 0;
volatile uint8_t g_data_available = 0;

void uart_init(void) {
    // Set baud rate (must match the transmitter)
    UBRRH = 0;
    UBRRL = 103; // For 9600 bps at 16MHz [14]

    // Enable receiver and transmitter
    UCSRB = (1 << RXEN) | (1 << TXEN);

    // Enable UART Receive Complete Interrupt
    UCSRB |= (1 << RXCIE);

    // Set frame format: 8 data bits, 1 stop bit, no parity
    UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
    sei(); // Enable global interrupts
}

uint8_t uart_has_new_data(void) {
    return g_data_available;
}

uint8_t uart_read_data(void) {
    g_data_available = 0;
    return g_received_data;
}

ISR(USART_RXC_vect) {
    // Read the received byte from the buffer
    g_received_data = UDR;
    // Set the flag to indicate new data is available
    g_data_available = 1;
}
