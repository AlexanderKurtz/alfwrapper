#include <alfwrapper/die.h>  // for die
#include <stdbool.h>         // for bool
#include <string.h>          // for strcmp, strdup, strncmp, NULL, size_t

bool string_equal (const char* a, const char* b) {
	return strcmp (a, b) == 0;
}

bool string_equal_prefix (const char* a, const char* b, size_t n) {
	return strncmp (a, b, n) == 0;
}

bool string_is_prefix (const char* a, const char* b) {
	return string_equal_prefix (a, b, strlen (a));
}

bool string_is_suffix (const char* a, const char* b) {
	return string_equal (a, b + strlen (b) - strlen (a));
}

char* string_duplicate (const char *s) {
	char* r = strdup (s);

	if (r == NULL) {
		die ("strdup(%s) failed", s);
	}

	return r;
}
