#ifndef COMMON_H_INC
#define COMMON_H_INC

#include <stdio.h>


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
