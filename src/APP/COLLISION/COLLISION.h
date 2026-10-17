/*
 * COLLISION.h
 *
 *  Created on: Sep 12, 2025 (PlatformIO port: Oct 17, 2026)
 *      Author: Abdelrahman Sherif Medhat
 */

#ifndef APP_COLLISION_COLLISION_H_
#define APP_COLLISION_COLLISION_H_

#include <stdint.h>

/* Hardware-independent warning logic: the caller feeds readings and the time,
 * then drives the LED and buzzer from the returned outputs. */
typedef struct{
	uint8_t u8LedOn;
	uint8_t u8BuzzerOn;
}COLLISION_tstrOutputs;

void                  COLLISION_voidInit(void);
void                  COLLISION_voidOnDistance(uint8_t Copy_u8DistanceCm, uint32_t Copy_u32NowMs);
COLLISION_tstrOutputs COLLISION_strUpdate(uint32_t Copy_u32NowMs);

#endif /* APP_COLLISION_COLLISION_H_ */
