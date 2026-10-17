/*
 * COLLISION_cfg.h
 *
 *  Created on: Sep 12, 2025 (PlatformIO port: Oct 17, 2026)
 *      Author: Abdelrahman Sherif Medhat
 */

#ifndef APP_COLLISION_COLLISION_CFG_H_
#define APP_COLLISION_COLLISION_CFG_H_

#define COLLISION_THRESHOLD_CM        50u    /* warn when distance <= 50 cm */
#define COLLISION_LED_TOGGLE_MS       500u   /* LED toggles every half second */
#define COLLISION_BUZZER_MS_PER_CM    10u    /* buzzer on, then off, for 10 x distance ms each */
#define COLLISION_LINK_TIMEOUT_MS     500u   /* no reading for this long -> stop warning */

#endif /* APP_COLLISION_COLLISION_CFG_H_ */
