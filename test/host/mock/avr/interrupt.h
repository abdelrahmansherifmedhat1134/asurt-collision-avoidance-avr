/* Host-test stand-in for <avr/interrupt.h>: ISRs become plain functions the test can call. */
#ifndef MOCK_AVR_INTERRUPT_H_
#define MOCK_AVR_INTERRUPT_H_

#define ISR(vector, ...)  void vector(void)
#define sei()             ((void)0)
#define cli()             ((void)0)

#endif /* MOCK_AVR_INTERRUPT_H_ */
