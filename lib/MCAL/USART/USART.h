/*
 * USART.h
 *
 *  Created on: Sep 12, 2025 (PlatformIO port: Oct 17, 2026)
 *      Author: Abdelrahman Sherif Medhat
 */

#ifndef MCAL_USART_USART_H_
#define MCAL_USART_USART_H_

#include <stdint.h>

void    USART_voidInit(void);                  /* 8N1, TX and RX enabled, polling TX */
void    USART_voidEnableRxInterrupt(void);     /* received bytes are latched by the RXC ISR */
void    USART_voidSendByte(uint8_t Copy_u8Data);
uint8_t USART_u8IsByteReceived(void);
uint8_t USART_u8ReadByte(void);                /* returns the latest byte and clears the flag */

#endif /* MCAL_USART_USART_H_ */
