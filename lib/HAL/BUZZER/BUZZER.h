/*
 * BUZZER.h
 *
 *  Created on: Sep 12, 2025 (PlatformIO port: Oct 17, 2026)
 *      Author: Abdelrahman Sherif Medhat
 */

#ifndef HAL_BUZZER_BUZZER_H_
#define HAL_BUZZER_BUZZER_H_

void BUZZER_voidInit(void);   /* PB3 output, buzzer off; owns Timer0 when BUZZER_USE_TONE = 1 */
void BUZZER_voidOn(void);
void BUZZER_voidOff(void);

#endif /* HAL_BUZZER_BUZZER_H_ */
