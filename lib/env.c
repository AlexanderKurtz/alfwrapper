#include <alfwrapper/die.h>  // for die
#include <stdlib.h>          // for setenv

void env_set (const char* name, const char* value) {
	int r = setenv (name, value, 1);

	if (r != 0) {
		die ("setenv (%s, %s, %d) failed", name, value, 1);
	}
}
