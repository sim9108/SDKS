#include <openssl/opensslconf.h>

#ifdef OPENSSL_FIPS
# include "fips_err.h"
#else

#ifdef OPENSSL_SYS_WINDOWS
static void *dummy=&dummy;
#endif

#endif
