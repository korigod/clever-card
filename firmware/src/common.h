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


#ifdef PRODUCTION
	#define ASSERT(x) ((void)0)
#else
	#ifdef TEST
		#define ASSERT(x) if ((x) == 0) { printf("\nASSERTION FAILED!\n"); for ( ;; ); }
	#else
		#ifdef configASSERT
			#define ASSERT(x) configASSERT(x)
		#else
			#define ASSERT(x) if ((x) == 0) { for ( ;; ); }
		#endif
	#endif
#endif

#ifdef DEBUG
	#define ASSERT_DEBUG(x) ASSERT(x)
#else
	#define ASSERT_DEBUG(x) ((void)0)
#endif

#ifdef NDEBUG
	#define ASSERT_RELEASE(x) ASSERT(x)
#else
	#define ASSERT_RELEASE(x) ((void)0)
#endif


#endif /* COMMON_H_INC */
