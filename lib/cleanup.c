#include <stdlib.h>  // for free

void cleanup_free (void* p) {
	/* We actually get a pointer to variable containing the pointer to be
	   freed but properly stating this in the parameter type makes
	   compilation fail with an incompatible pointer warning if used for
	   something other than void*.
	 */
	free (* (void**) p);
}
