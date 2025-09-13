/*
 * actuator_drivers.h
 *
 *  Created on: Sep 12, 2025
 *      Author: User
 */

#ifndef DRIVERS_ACTUATOR_DRIVERS_H
#define DRIVERS_ACTUATOR_DRIVERS_H

#include <avr/io.h>
#include <avr/interrupt.h>

void led_init(void);
void buzzer_init(void);
void led_toggle(void);
void buzzer_on(void);
void buzzer_off(void);

#endif /* DRIVERS_ACTUATOR_DRIVERS_H_ */
