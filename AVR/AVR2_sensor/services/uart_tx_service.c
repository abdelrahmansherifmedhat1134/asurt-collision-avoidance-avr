#include "uart_tx_service.h"

void uart_init(void) {
    // Set baud rate [14]
    UBRRH = (uint8_t)(BAUD_PRESCALLER >> 8);
    UBRRL = (uint8_t)(BAUD_PRESCALLER);

    // Enable transmitter and receiver (though we only need TX) [16]
    UCSRB = (1 << RXEN) | (1 << TXEN);

    // Set frame format: 8 data bits, 1 stop bit, no parity [13]
    UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
}

void uart_send_byte(uint8_t data) {
    // Wait for empty transmit buffer [13]
    while (!(UCSRA & (1 << UDRE)));

    // Put data into buffer, sends the data
    UDR = data;
}

