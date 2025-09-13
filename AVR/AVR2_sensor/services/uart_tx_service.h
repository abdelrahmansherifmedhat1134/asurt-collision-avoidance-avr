/*
 * uart_tx_service.h
 *
 *  Created on: Sep 12, 2025
 *      Author: User
 */

#ifndef SERVICES_UART_TX_SERVICE_H
#define SERVICES_UART_TX_SERVICE_H

#include <avr/io.h>
#include <stdint.h>

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

void uart_init(void);
void uart_send_byte(uint8_t data);

#endif /* SERVICES_UART_TX_SERVICE_H_ */
