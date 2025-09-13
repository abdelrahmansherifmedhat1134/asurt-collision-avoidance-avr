#include "collision_service.h"
#include <util/delay.h>

// Buzzer State Machine variables
typedef enum {
    BUZZER_IDLE,
    BUZZER_ACTIVE,
} BuzzerState;

BuzzerState g_buzzer_state = BUZZER_IDLE;
uint16_t g_buzzer_duration = 0;
uint32_t g_buzzer_start_time = 0;

void collision_service_update(uint8_t distance) {
    // State machine for the buzzer
    switch (g_buzzer_state) {
        case BUZZER_IDLE:
            if (distance <= 50) {
                // Transition to active state
                g_buzzer_state = BUZZER_ACTIVE;
                g_buzzer_duration = (uint16_t)distance * 10;
                buzzer_on();
                g_buzzer_start_time = (uint32_t)millis();
            }
            break;

        case BUZZER_ACTIVE:
            // Check if duration has elapsed
            if ((uint32_t)millis() - g_buzzer_start_time >= g_buzzer_duration) {
                // Transition to idle state
                buzzer_off();
                g_buzzer_state = BUZZER_IDLE;
            }
            break;
    }
}
