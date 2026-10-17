/*
 * COLLISION.c
 *
 *  Created on: Sep 12, 2025 (PlatformIO port: Oct 17, 2026)
 *      Author: Abdelrahman Sherif Medhat
 */
#include "COLLISION.h"
#include "COLLISION_cfg.h"

static uint8_t               COLLISION_u8HaveReading;
static uint8_t               COLLISION_u8DistanceCm;
static uint32_t              COLLISION_u32ReadingMs;
static uint8_t               COLLISION_u8Active;
static uint32_t              COLLISION_u32LedToggleMs;
static uint32_t              COLLISION_u32BuzzerToggleMs;
static COLLISION_tstrOutputs COLLISION_strOut;

void COLLISION_voidInit(void){
	COLLISION_u8HaveReading     = 0;
	COLLISION_u8Active          = 0;
	COLLISION_strOut.u8LedOn    = 0;
	COLLISION_strOut.u8BuzzerOn = 0;
}

void COLLISION_voidOnDistance(uint8_t Copy_u8DistanceCm, uint32_t Copy_u32NowMs){
	COLLISION_u8DistanceCm  = Copy_u8DistanceCm;
	COLLISION_u32ReadingMs  = Copy_u32NowMs;
	COLLISION_u8HaveReading = 1;
}

COLLISION_tstrOutputs COLLISION_strUpdate(uint32_t Copy_u32NowMs){
	uint8_t  Local_u8Near;
	uint16_t Local_u16BuzzerHalfMs;

	Local_u8Near = COLLISION_u8HaveReading
	            && (COLLISION_u8DistanceCm <= COLLISION_THRESHOLD_CM)
	            && ((Copy_u32NowMs - COLLISION_u32ReadingMs) < COLLISION_LINK_TIMEOUT_MS);

	if(!Local_u8Near){
		COLLISION_u8Active          = 0;
		COLLISION_strOut.u8LedOn    = 0;
		COLLISION_strOut.u8BuzzerOn = 0;
		return COLLISION_strOut;
	}

	if(!COLLISION_u8Active){                 /* obstacle just entered the warning zone */
		COLLISION_u8Active          = 1;
		COLLISION_strOut.u8LedOn    = 1;
		COLLISION_strOut.u8BuzzerOn = 1;
		COLLISION_u32LedToggleMs    = Copy_u32NowMs;
		COLLISION_u32BuzzerToggleMs = Copy_u32NowMs;
		return COLLISION_strOut;
	}

	if((Copy_u32NowMs - COLLISION_u32LedToggleMs) >= COLLISION_LED_TOGGLE_MS){
		COLLISION_strOut.u8LedOn ^= 1;
		COLLISION_u32LedToggleMs += COLLISION_LED_TOGGLE_MS;
	}

	/* the latest reading sets the buzzer period; 0 cm keeps it on continuously */
	Local_u16BuzzerHalfMs = (uint16_t)COLLISION_u8DistanceCm * COLLISION_BUZZER_MS_PER_CM;
	if(Local_u16BuzzerHalfMs == 0){
		COLLISION_strOut.u8BuzzerOn = 1;
	}else if((Copy_u32NowMs - COLLISION_u32BuzzerToggleMs) >= Local_u16BuzzerHalfMs){
		COLLISION_strOut.u8BuzzerOn ^= 1;
		COLLISION_u32BuzzerToggleMs  = Copy_u32NowMs;
	}
	return COLLISION_strOut;
}
