/*
 * LED.c
 *
 *  Created on: Sep 12, 2025 (PlatformIO port: Oct 17, 2026)
 *      Author: Abdelrahman Sherif Medhat
 */
#include <avr/io.h>
#include "LED.h"
#include "LED_cfg.h"

void LED_voidInit(void){
	LED_PORT &= ~(1 << LED_PIN);
	LED_DDR  |=  (1 << LED_PIN);
}

void LED_voidOn(void){
	LED_PORT |= (1 << LED_PIN);
}

void LED_voidOff(void){
	LED_PORT &= ~(1 << LED_PIN);
}
