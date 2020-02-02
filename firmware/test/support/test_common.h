#ifndef TEST_COMMON_H_INC
#define TEST_COMMON_H_INC


#ifdef TEST_PREPROCESS
#   define TEST_VALUE(...) TEST_CASE(__VA_ARGS__)
#else
#   define TEST_VALUE(...)
#endif


extern void __attribute__((weak)) additionalSuiteSetUp(void);


#endif /* TEST_COMMON_H_INC */
