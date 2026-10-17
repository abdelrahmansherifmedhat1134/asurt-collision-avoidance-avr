/* Host-test stand-in for <util/atomic.h>: the block runs once. */
#ifndef MOCK_UTIL_ATOMIC_H_
#define MOCK_UTIL_ATOMIC_H_

#define ATOMIC_RESTORESTATE  0
#define ATOMIC_BLOCK(type)   for(int mock_atomic_once = 1; mock_atomic_once; mock_atomic_once = 0)

#endif /* MOCK_UTIL_ATOMIC_H_ */
