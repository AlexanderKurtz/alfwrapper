#include <alfwrapper/die.h>  // for die
#include <stdint.h>          // for intmax_t
#include <stdio.h>           // for snprintf, size_t
#include <unistd.h>          // for close, dup2, execvp, fork, getpid, pid_t

/* getpid(), but as a statically allocated string */
char* pid_string (){
	static char buffer[sizeof (intmax_t) * 3 /* max decimals per byte */ + 1 /* sign */ + 1 /* null byte */];
	int characters = snprintf (buffer, sizeof (buffer), "%jd", (intmax_t) getpid ());

	if ((size_t) characters >= sizeof (buffer)) {
		die ("snprintf(%p, %zu, %s, %ld) failed", buffer, sizeof (buffer), "%jd", (intmax_t) getpid ());
	}

	return buffer;
}
