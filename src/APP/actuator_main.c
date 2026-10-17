/*
 * actuator_main.c  (AVR 1)
 *
 *  Receives distances over UART and drives the LED and buzzer warning.
 *  Timer use: Timer0 = buzzer tone, Timer2 = 1 ms system tick.
 *
 *  Created on: Sep 12, 2025 (PlatformIO port: Oct 17, 2026)
 *      Author: Abdelrahman Sherif Medhat
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "USART/USART.h"
#include "LED/LED.h"
#include "BUZZER/BUZZER.h"
#include "SYSTICK/SYSTICK.h"
#include "COLLISION/COLLISION.h"

int main(void){
	uint32_t              Local_u32NowMs;
	COLLISION_tstrOutputs Local_strOut;

	LED_voidInit();
	BUZZER_voidInit();
	SYSTICK_voidInit();
	USART_voidInit();
	USART_voidEnableRxInterrupt();
	COLLISION_voidInit();
	sei();

	while(1){
		Local_u32NowMs = SYSTICK_u32Millis();
		if(USART_u8IsByteReceived()){
			COLLISION_voidOnDistance(USART_u8ReadByte(), Local_u32NowMs);
		}
		Local_strOut = COLLISION_strUpdate(Local_u32NowMs);

		if(Local_strOut.u8LedOn)    { LED_voidOn();    } else { LED_voidOff();    }
		if(Local_strOut.u8BuzzerOn) { BUZZER_voidOn(); } else { BUZZER_voidOff(); }
	}
}
