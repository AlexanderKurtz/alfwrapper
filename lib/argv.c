#include <alfwrapper/argv.h>
#include <alfwrapper/die.h>     // for die
#include <alfwrapper/string.h>  // for string_equal, string_equal_prefix
#include <stdbool.h>            // for true, bool, false
#include <stdio.h>              // for NULL, size_t
#include <string.h>             // for strlen, strstr

static void drop (char** argv, int index) {
	for (int i = index; argv[i]; i++) {
		argv[i] = argv[i + 1];
	}
}

static bool core (char** argv, char* option, argv_handler handle) {
	for (int i = 1; argv[i] && argv[i][0] == '-' && !string_equal (argv[i], "--"); i++) {
		size_t len = strlen (option);

		if (string_equal (argv[i], option)) {
			/* --option foo */
			if (argv[i + 1]) {
				handle (argv[i + 1]);
				drop (argv, i);
				drop (argv, i);
				return true;
			} else {
				die ("Option '%s' needs an argument", option);
			}
		} else if (string_equal_prefix (argv[i], option, len) && argv[i][len] == '=') {
			/* --option=foo */
			handle (argv[i] + len + 1);
			drop (argv, i);
			return true;
		} else if (strstr (argv[i], "=") == NULL) {
			/* Skip argument of ignored option */
			i++;
		}
	}

	return false;
}

void argv_finish (char** argv, argv_handler handle) {
	if (argv[1] && argv[1][0] == '-') {
		if (string_equal (argv[1], "--")) {
			drop (argv, 1);
		} else {
			handle (argv[1]);
		}
	}
}

void argv_option (char** argv, char* option, argv_handler handle) {
	while (core (argv, option, handle));
}
