#ifndef MOCK_REGS_H_
#define MOCK_REGS_H_

#include <stdint.h>

#define MOCK_NO_EDGE  0xFFFFu

void     mock_reset_registers(void);
void     mock_set_echo(uint16_t rise_tick, uint16_t fall_tick);   /* Timer1 ticks after start */
uint16_t mock_timer1_ticks(void);

#endif /* MOCK_REGS_H_ */
