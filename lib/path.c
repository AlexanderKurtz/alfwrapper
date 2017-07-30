#define _GNU_SOURCE

#include <alfwrapper/die.h>     // for die
#include <alfwrapper/string.h>  // for string_duplicate
#include <libgen.h>             // for dirname, basename
#include <stddef.h>             // for NULL
#include <unistd.h>             // for chdir, get_current_dir_name

char* path_basename (const char* path) {
	char* r = basename (string_duplicate (path));

	if (r == NULL) {
		die ("basename (%s) failed", path);
	}

	return r;
}

char* path_dirname (const char* path) {
	char* r = dirname (string_duplicate (path));

	if (r == NULL) {
		die ("basename (%s) failed", path);
	}

	return r;
}

char* path_get_current () {
	char* cwd = get_current_dir_name ();

	if (cwd == NULL) {
		die ("get_current_dir_name() returned NULL");
	}

	return cwd;
}

void path_change (const char* path) {
	int r = chdir (path);

	if (r != 0) {
		die ("chdir (%s) failed", path);
	}
}
