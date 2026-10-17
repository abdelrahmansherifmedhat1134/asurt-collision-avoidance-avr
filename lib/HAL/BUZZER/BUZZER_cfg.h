/*
 * BUZZER_cfg.h
 *
 *  Created on: Sep 12, 2025 (PlatformIO port: Oct 17, 2026)
 *      Author: Abdelrahman Sherif Medhat
 */

#ifndef HAL_BUZZER_BUZZER_CFG_H_
#define HAL_BUZZER_BUZZER_CFG_H_

/* 1: passive buzzer/speaker, driven with a 50 % square-wave tone on OC0 (PB3) by Timer0
 * 0: active buzzer, PB3 is simply driven high or low                                  */
#define BUZZER_USE_TONE       1

/* Timer0 CTC, F_CPU/64, toggle OC0 on compare: f = F_CPU / (2 * 64 * (1 + OCR0)) = 1 kHz */
#define BUZZER_TONE_OCR0      124

#endif /* HAL_BUZZER_BUZZER_CFG_H_ */
