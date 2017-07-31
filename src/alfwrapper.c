#include <alfwrapper/argv.h>                    // for argv_option, argv_finish
#include <alfwrapper/bcc.h>                     // for bcc_attach, bcc_load
#include <alfwrapper/die.h>                     // for die
#include <alfwrapper/parse.h>                   // for mapspec, parse_mapspec
#include <alfwrapper/socket.h>                  // for socket_bind, socket_c...
#include <stddef.h>                             // for NULL
#include <stdlib.h>                             // for realloc
#include <sys/socket.h>                         // for SOCK_STREAM

static const char*    address   = "::";
static const char*    port      = "4242";
static socktype       type      = SOCK_STREAM;
static int            set_count = 0;
static mapspec*       set       = NULL;

static void handle_address (const char* input) {
	address = input;
}

static void handle_port (const char* input) {
	port = input;
}

static void handle_type (const char* input) {
	parse_socktype (input, &type);
}

static void handle_set (const char* input) {
	set = realloc (set, sizeof (*set) * ++set_count);
	parse_mapspec (input, set + set_count - 1);
}

static void handle_unknown (const char* input) {
	die ("Unrecognized option: %s", input);
}

int main (int argc, char** argv) {
	/* Ignore argc, argv is NULL terminated anyway */
	(void) argc;

	/* Handle potential command line options */
	argv_option (argv, "--address",  handle_address);
	argv_option (argv, "--port",     handle_port);
	argv_option (argv, "--set",      handle_set);
	argv_option (argv, "--type",     handle_type);
	argv_finish (argv,               handle_unknown);

	/* Make sure we received a filter path and main program */
	if (argv[1] == NULL || argv[2] == NULL) {
		die ("Usage: %s [--address ADDRESS] [--port PORT] [--type TYPE] [--set MAPNAME,KEY_TYPE:KEY_STRING,VALUE_TYPE:VALUE_STRING] <filter> <program> [<argument>] [...]", argv[0]);
	}

	/* Look up the address and port */
	struct addrinfo* addrinfo = socket_lookup (address, port, type);

	/* Create the server socket */
	int sockfd = socket_create (addrinfo);

	/* Load the filter program */
	void* module = bcc_load (argv[1]);

	/* Set up the maps in the filter program */
	for (int i = 0; i < set_count; i++) {
		bcc_set (module, set[i].map, &set[i].key, &set[i].val);
	}

	/* Attach the socket to the filter program */
	bcc_attach (sockfd, module, "filter");

	/* Bind the socket to the specified address */
	socket_bind (sockfd, addrinfo);

	/* If possible, start listening on the socket */
	if (type == SOCK_STREAM) {
		socket_listen (sockfd);
	}

	/* Pass the socket to the main program */
	socket_pass (sockfd, &argv[2]);
}
