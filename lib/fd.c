#include <alfwrapper/die.h>  // for die
#include <unistd.h>          // for close

/* close() with error checking */
void fd_close (int fd) {
	int r = close (fd);

	if (r < 0) {
		die ("close(%d) failed", fd);
	}
}
