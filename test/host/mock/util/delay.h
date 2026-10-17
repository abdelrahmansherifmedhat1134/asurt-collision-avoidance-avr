/* Host-test stand-in for <util/delay.h>: delays take no time. */
#ifndef MOCK_UTIL_DELAY_H_
#define MOCK_UTIL_DELAY_H_

#define _delay_us(us)  ((void)(us))
#define _delay_ms(ms)  ((void)(ms))

#endif /* MOCK_UTIL_DELAY_H_ */
