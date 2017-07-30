#include <alfwrapper/die.h>  // for die
#include <stdbool.h>         // for bool
#include <string.h>          // for strcmp, strdup, strncmp, NULL, size_t

bool string_equal (const char* a, const char* b) {
	return strcmp (a, b) == 0;
}

bool string_equal_prefix (const char* a, const char* b, size_t n) {
	return strncmp (a, b, n) == 0;
}

char* string_duplicate (const char *s) {
	char* r = strdup (s);

	if (r == NULL) {
		die ("strdup(%s) failed", s);
	}

	return r;
}
