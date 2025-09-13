/*
 * uart_rx_service.h
 *
 *  Created on: Sep 12, 2025
 *      Author: User
 */

#ifndef SERVICES_UART_RX_SERVICE_H
#define SERVICES_UART_RX_SERVICE_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

void uart_init(void);
uint8_t uart_has_new_data(void);
uint8_t uart_read_data(void);


#endif /* SERVICES_UART_RX_SERVICE_H_ */
