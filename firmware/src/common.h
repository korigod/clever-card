#ifndef COMMON_H_INC
#define COMMON_H_INC

#include <stdio.h>

#define min(a,b) __extension__ ({ \
	__typeof__ (a) _a = (a); \
	__typeof__ (b) _b = (b); \
	_a < _b ? _a : _b; \
})

#define max(a,b) __extension__ ({ \
	__typeof__ (a) _a = (a); \
	__typeof__ (b) _b = (b); \
	_a > _b ? _a : _b; \
})


#ifdef TEST
	#ifdef TEST_TARGET
		#define ASSERT(x) EFM_ASSERT(x)
	#else
		#define ASSERT(x) ((x) ? ((void)0) : printf("Assertion failed!"))
	#endif
#else
	#define ASSERT(x) ((void)0)
#endif


#endif /* COMMON_H_INC */
