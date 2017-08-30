#include <alfwrapper/socket.h>
#include <alfwrapper/die.h>  // for die
#include <alfwrapper/env.h>  // for env_set
#include <alfwrapper/fd.h>   // for fd_close
#include <alfwrapper/pid.h>  // for pid_string
#include <netdb.h>           // for addrinfo, getaddrinfo
#include <stdio.h>           // for NULL, size_t
#include <sys/socket.h>      // for bind, connect, listen, setsockopt, socket

/* getaddrinfo() with error checking. */
struct addrinfo* socket_lookup (const char* addr, const char* port, socktype type) {
	struct addrinfo* result = NULL;

	struct addrinfo hints = {
		.ai_flags     = 0,
		.ai_family    = AF_INET6,
		.ai_socktype  = type,
		.ai_protocol  = 0,
		.ai_addrlen   = 0,
		.ai_addr      = NULL,
		.ai_canonname = NULL,
		.ai_next      = NULL,
	};

	int r = getaddrinfo (addr, port, &hints, &result);

	if (r != 0) {
		die ("getaddrinfo(%s, %s, %p, %p) failed", addr, port, &hints, &result);
	}

	if (result == NULL) {
		die ("getaddrinfo(%s, %s, %p, %p) returned no addresses", addr, port, &hints, &result);
	}

	return result;
}

/* bind() with error checking. */
void socket_bind (int sockfd, struct addrinfo* addr) {
	int one = 1;

	/* Enable reuse of recently freed ports */
	socket_option (sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof (one));

	int r = bind (sockfd, addr->ai_addr, addr->ai_addrlen);

	if (r != 0) {
		die ("bind (%d, %p, %zu) failed", sockfd, addr->ai_addr, (size_t) addr->ai_addrlen);
	}
}

/* connect() with error checking */
void socket_connect (int sockfd, struct addrinfo* addr) {
	int r = connect (sockfd, addr->ai_addr, addr->ai_addrlen);

	if (r != 0) {
		die ("connect (%d, %p, %zu) failed", sockfd, addr->ai_addr, (size_t) addr->ai_addrlen);
	}
}

int socket_create (struct addrinfo* addrinfo) {
	int sockfd = socket (addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol);

	if (sockfd < 0) {
		die ("socket (%d, %d, %d) failed", addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol);
	}

	return sockfd;
}

/* listen() with error checking */
void socket_listen (int sockfd) {
	int r = listen (sockfd, 10);

	if (r != 0) {
		die ("listen (%d) failed", sockfd);
	}
}

/* setsockopt() with error checking */
void socket_option (int sockfd, int level, int optname, void* optval, socklen_t optlen) {
	int r = setsockopt (sockfd, level, optname, optval, optlen);

	if (r < 0) {
		die ("setsockopt (%d, %d, %d, %p, %zu) failed", sockfd, level, optname, optval, (size_t) optlen);
	}
}

/* execvp() with error checking */
void socket_pass (int sockfd, char** arguments) {
	if (sockfd != 3) {
		die ("socket_pass (%d, %p): The systemd socket activation protocol requires socket at FD #3", sockfd, arguments);
	}

	if (arguments == NULL) {
		die ("replace_programm(%p): Need non-NULL argument", arguments);
	}

	if (arguments[0] == NULL) {
		die ("replace_programm(%p): Need non-empty argument", arguments);
	}

	env_set ("LISTEN_FDS", "1");
	env_set ("LISTEN_PID", pid_string ());

	execvp (arguments[0], arguments);

	die ("execvp(%s, %p) failed", arguments[0], arguments);
}

/* fork() and replace_program() with error checking */
pid_t socket_fork_pass (int sockfd, char** arguments) {
	pid_t pid = fork ();

	if (pid < 0) {
		die ("fork() failed");
	} else if (pid == 0) {
		/* We are the child, so exec */
		socket_pass (sockfd, arguments);
	}

	/* We are the parent, so close the socket and return */
	fd_close (sockfd);
	return pid;
}
