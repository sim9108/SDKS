
#include <stdlib.h>
#include "webp/types.h"

WEBP_EXTERN(void) WebPFree(void* ptr) {
	free(ptr);
}