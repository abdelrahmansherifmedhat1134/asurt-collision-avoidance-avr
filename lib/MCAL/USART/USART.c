/*
 * USART.c
 *
 *  Created on: Sep 12, 2025 (PlatformIO port: Oct 17, 2026)
 *      Author: Abdelrahman Sherif Medhat
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "USART.h"
#include "USART_cfg.h"

#define USART_UBRR_VALUE   ((F_CPU / (16UL * USART_BAUD_RATE)) - 1UL)   /* 103 at 16 MHz, 9600 baud */

static volatile uint8_t USART_u8RxData;
static volatile uint8_t USART_u8RxFlag;

void USART_voidInit(void){
	UBRRH = (uint8_t)(USART_UBRR_VALUE >> 8);          /* URSEL = 0 selects UBRRH */
	UBRRL = (uint8_t)(USART_UBRR_VALUE);
	UCSRB = (1 << RXEN) | (1 << TXEN);
	UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0); /* URSEL = 1 selects UCSRC: 8 data bits */
}

void USART_voidEnableRxInterrupt(void){
	UCSRB |= (1 << RXCIE);
}

void USART_voidSendByte(uint8_t Copy_u8Data){
	while(!(UCSRA & (1 << UDRE)));
	UDR = Copy_u8Data;
}

uint8_t USART_u8IsByteReceived(void){
	return USART_u8RxFlag;
}

uint8_t USART_u8ReadByte(void){
	uint8_t Local_u8Data;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		Local_u8Data   = USART_u8RxData;
		USART_u8RxFlag = 0;
	}
	return Local_u8Data;
}

ISR(USART_RXC_vect){
	USART_u8RxData = UDR;
	USART_u8RxFlag = 1;
}
