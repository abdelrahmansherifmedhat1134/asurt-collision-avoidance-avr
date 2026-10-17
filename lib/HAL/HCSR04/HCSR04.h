/*
 * HCSR04.h
 *
 *  Created on: Sep 12, 2025 (PlatformIO port: Oct 17, 2026)
 *      Author: Abdelrahman Sherif Medhat
 */

#ifndef HAL_HCSR04_HCSR04_H_
#define HAL_HCSR04_HCSR04_H_

#include <stdint.h>

#define HCSR04_NO_ECHO        0xFFFFu

void     HCSR04_voidInit(void);                         /* Timer1 stays stopped between measurements */
uint16_t HCSR04_u16MeasureCm(void);                     /* blocking, at most HCSR04_TIMEOUT_MS; HCSR04_NO_ECHO on timeout */
uint16_t HCSR04_u16TicksToCm(uint16_t Copy_u16Ticks);   /* echo width in Timer1 ticks -> cm */

#endif /* HAL_HCSR04_HCSR04_H_ */
