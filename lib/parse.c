#include <alfwrapper/cleanup.h>  // for _cleanup_free_
#include <alfwrapper/data.h>     // for address6, address4, interface, subnet4
#include <alfwrapper/die.h>      // for die
#include <alfwrapper/parse.h>    // for key_value_type, parameter
#include <alfwrapper/socket.h>   // for socktype
#include <alfwrapper/string.h>   // for string_is_prefix, string_duplicate
#include <arpa/inet.h>           // for inet_pton
#include <endian.h>              // for be64toh, be32toh
#include <net/if.h>              // for if_nametoindex
#include <stdbool.h>             // for false
#include <stdlib.h>              // for atoi, NULL
#include <string.h>              // for strsep
#include <sys/socket.h>          // for AF_INET, AF_INET6, SOCK_DGRAM, SOCK_...

static void parse_address6 (const char* input, struct address6* output) {
	int r = inet_pton (AF_INET6, input, output);

	if (r != 1) {
		die ("inet_pton (AF_INET6, %s, %p) failed", input, output);
	}

	/* Yes, we want it that way */
	output->high = be64toh (output->high);
	output->low  = be64toh (output->low);
}

static void parse_subnet6 (const char* input, struct subnet6* output) {
	char* copy _cleanup_free_ = string_duplicate (input);
	char* data = copy;

	char* prefix = strsep (&data, "/");
	parse_address6 (prefix, &output->address);
	output->prefix = atoi (data);
}

static void parse_interface (const char* input, struct interface* output) {
	output->raw = if_nametoindex (input);

	if (output->raw == 0) {
		die ("if_nametoindex(%s) failed", input);
	}
}

static void parse_portnumber (const char* input, struct portnumber* output) {
	output->raw = atoi (input);
}

static void parse_index (const char* input, struct index* output) {
	output->raw = atoi (input);
}

void parse_typed (const char* type, const char* data, union key_value_type* output) {
	if (!string_is_suffix ("\"struct\"]", type)) {
		die ("parsed_typed (%s, %s, %p): Type is not a struct", type, data, output);
	}

	if (false) {}
	else if (string_is_prefix ("[\"address6\"",   type)) { parse_address6   (data, &output->address6_member);   }
	else if (string_is_prefix ("[\"interface\"",  type)) { parse_interface  (data, &output->interface_member);  }
	else if (string_is_prefix ("[\"portnumber\"", type)) { parse_portnumber (data, &output->portnumber_member); }
	else if (string_is_prefix ("[\"subnet6\"",    type)) { parse_subnet6    (data, &output->subnet6_member);    }
	else if (string_is_prefix ("[\"index\"",      type)) { parse_index      (data, &output->index_member);      }
	else { die ("parse_typed (%s, %s, %p): Unrecognized type", type, data, output); }
}

void parse_parameter (const char* input, struct parameter* output) {
	char* data = string_duplicate (input);

	const char* table = strsep (&data, ",");
	const char* key   = strsep (&data, ",");
	const char* value = strsep (&data, ",");

	if (table == NULL || key == NULL || value == NULL) {
		die ("Invalid parameter specification: %s", input);
	}

	output->table = table;
	output->key   = key;
	output->value = value;
}

void parse_socktype (const char* input, socktype* output) {
	if (string_equal (input, "SOCK_DGRAM")) {
		*output = SOCK_DGRAM;
	} else if (string_equal (input, "SOCK_STREAM")) {
		*output = SOCK_STREAM;
	} else {
		die ("parse_socktype (%s, %p): Unknown socket type", input, output);
	}
}
