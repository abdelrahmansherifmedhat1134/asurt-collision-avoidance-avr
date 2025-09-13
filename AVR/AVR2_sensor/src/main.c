#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "C:\temp\AVR2_sensor\drivers\hcsr04_driver.h"
#include "C:\temp\AVR2_sensor\services\uart_tx_service.h"

int main(void) {
    // Define the CPU frequency for the delay functions
    #define F_CPU 16000000UL

    // Initialize the ultrasonic sensor and UART
    hcsr04_init();
    uart_init();

    // Loop continuously
    while (1) {
        // Trigger the sensor to start a measurement
        hcsr04_trigger_pulse();

        // Wait for the measurement to be ready. This is a non-blocking poll.
        // The Input Capture ISR handles the timing in the background.
        while (!hcsr04_is_measurement_ready());

        // Read the calculated distance in cm
        uint16_t distance = hcsr04_read_distance_cm();

        // Clamp the distance to a safe value to avoid overflow on the receiving end
        if (distance > 255) {
            distance = 255;
        }

        // Transmit the distance value over UART
        uart_send_byte((uint8_t)distance);

        // Wait a short time before the next measurement to prevent echo interference
        _delay_ms(100);
    }

    return 0;
}

