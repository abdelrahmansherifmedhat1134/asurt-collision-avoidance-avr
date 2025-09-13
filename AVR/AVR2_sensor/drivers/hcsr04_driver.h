/*

 *
 *  Created on: Sep 12, 2025
 *      Author: User
 */

#ifndef DRIVERS_HCSR04_DRIVER_H
#define DRIVERS_HCSR04_DRIVER_H

#include <avr/io.h>
#include <avr/interrupt.h>

void hcsr04_init(void);
uint16_t hcsr04_read_distance_cm(void);
void hcsr04_trigger_pulse(void);
uint8_t hcsr04_is_measurement_ready(void);
void hcsr04_reset_measurement_flag(void);

#endif /* DRIVERS_HCSR04_DRIVER_H_ */
