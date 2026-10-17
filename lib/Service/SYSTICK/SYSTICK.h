/*
 * SYSTICK.h
 *
 *  Created on: Sep 12, 2025 (PlatformIO port: Oct 17, 2026)
 *      Author: Abdelrahman Sherif Medhat
 */

#ifndef SERVICE_SYSTICK_SYSTICK_H_
#define SERVICE_SYSTICK_SYSTICK_H_

#include <stdint.h>

void     SYSTICK_voidInit(void);       /* Timer2 CTC, 1 ms interrupt */
uint32_t SYSTICK_u32Millis(void);

#endif /* SERVICE_SYSTICK_SYSTICK_H_ */
