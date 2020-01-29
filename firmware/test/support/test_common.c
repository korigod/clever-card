#include "test_common.h"

#ifdef TEST_TARGET
	#include "swo.h"
	#include "retargetio.h"

	void suiteSetUp(void) {
		setupSWOForPrint();
	}
#endif
