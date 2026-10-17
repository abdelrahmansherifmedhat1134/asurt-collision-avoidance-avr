/*
 * HCSR04_cfg.h
 *
 *  Created on: Sep 12, 2025 (PlatformIO port: Oct 17, 2026)
 *      Author: Abdelrahman Sherif Medhat
 */

#ifndef HAL_HCSR04_HCSR04_CFG_H_
#define HAL_HCSR04_HCSR04_CFG_H_

/* TRIG can be any output pin. ECHO must be on ICP1 (PD6): it is timed by Timer1 input capture. */
#define HCSR04_TRIG_DDR       DDRA
#define HCSR04_TRIG_PORT      PORTA
#define HCSR04_TRIG_PIN       PA6

/* The HC-SR04 holds ECHO high for about 38 ms when nothing is in range */
#define HCSR04_TIMEOUT_MS     40UL

#endif /* HAL_HCSR04_HCSR04_CFG_H_ */
