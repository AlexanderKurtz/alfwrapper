#include <errno.h>   // for errno
#include <stdarg.h>  // for va_end, va_list, va_start
#include <stdio.h>   // for fprintf, stderr, vfprintf
#include <stdlib.h>  // for exit
#include <string.h>  // for strerror

/* Print an error message and terminate the program */
void die (const char* format, ...) {
	fprintf (stderr, "Fatal error: ");

	va_list args;
	va_start (args, format);
	vfprintf (stderr, format, args);
	va_end (args);

	if (errno == 0) {
		fprintf (stderr, "\n");
	} else {
		fprintf (stderr, ": %s\n", strerror (errno));
	}

	exit (1);
}
