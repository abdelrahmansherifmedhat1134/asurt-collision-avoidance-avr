/*
 * sensor_main.c  (AVR 2)
 *
 *  Measures the distance every 100 ms and sends it to AVR 1 as one byte (cm, 255 = out of range).
 *  Timer use: Timer1 = HC-SR04 echo capture.
 *
 *  Created on: Sep 12, 2025 (PlatformIO port: Oct 17, 2026)
 *      Author: Abdelrahman Sherif Medhat
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "USART/USART.h"
#include "HCSR04/HCSR04.h"

#define SENSOR_PERIOD_MS     100     /* the HC-SR04 needs at least 60 ms between measurements */
#define SENSOR_MAX_CM        255u

int main(void){
	uint16_t Local_u16DistanceCm;

	HCSR04_voidInit();
	USART_voidInit();
	sei();

	while(1){
		Local_u16DistanceCm = HCSR04_u16MeasureCm();       /* HCSR04_NO_ECHO also clamps to 255 */
		if(Local_u16DistanceCm > SENSOR_MAX_CM){
			Local_u16DistanceCm = SENSOR_MAX_CM;
		}
		USART_voidSendByte((uint8_t)Local_u16DistanceCm);
		_delay_ms(SENSOR_PERIOD_MS);
	}
}
