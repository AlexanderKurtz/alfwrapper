#include <alfwrapper/cleanup.h>  // for _cleanup_free_
#include <alfwrapper/die.h>      // for die
#include <alfwrapper/parse.h>    // for key_value_type, address6, mapspec
#include <alfwrapper/string.h>   // for string_equal, string_duplicate
#include <arpa/inet.h>           // for inet_pton
#include <endian.h>              // for __x, be64toh, be32toh
#include <net/if.h>              // for if_nametoindex
#include <stdbool.h>             // for false
#include <stdint.h>              // for uint32_t
#include <stdlib.h>              // for atoi
#include <string.h>              // for strsep
#include <sys/socket.h>          // for AF_INET, AF_INET6

static void parse_address4 (const char* input, address4* output) {
	int r = inet_pton (AF_INET, input, output);

	if (r != 1) {
		die ("inet_pton (AF_INET, %s, %p) failed", input, output);
	}

	/* Yes, we want it that way */
	*output = be32toh (*output);
}

static void parse_address6 (const char* input, address6* output) {
	int r = inet_pton (AF_INET6, input, output);

	if (r != 1) {
		die ("inet_pton (AF_INET6, %s, %p) failed", input, output);
	}

	/* Yes, we want it that way */
	output->high = be64toh (output->high);
	output->low  = be64toh (output->low);
}

static void parse_subnet4 (const char* input, subnet4* output) {
	char* copy _cleanup_free_ = string_duplicate (input);
	char* data = copy;

	char* prefix = strsep (&data, "/");
	parse_address4 (prefix, &output->address);
	output->prefix = atoi (data);
}

static void parse_subnet6 (const char* input, subnet6* output) {
	char* copy _cleanup_free_ = string_duplicate (input);
	char* data = copy;

	char* prefix = strsep (&data, "/");
	parse_address6 (prefix, &output->address);
	output->prefix = atoi (data);
}

static void parse_interface (const char* input, interface* output) {
	*output = if_nametoindex (input);

	if (*output == 0) {
		die ("if_nametoindex(%s) failed", input);
	}
}

static void parse_portnumber (const char* input, portnumber* output) {
	*output = atoi (input);
}

static void parse_uint32_t (const char* input, uint32_t* output) {
	*output = atoi (input);
}

static void parse_typed (const char* input, key_value_type* output) {
	char* copy _cleanup_free_ = string_duplicate (input);
	char* data = copy;

	char* type  = strsep (&data, ":");

	if (!type || !data) {
		die ("Invalid typed data: %s", input);
	}

	if (false) {}
	else if (string_equal (type, "address4"))   { parse_address4   (data, &output->address4_member);   }
	else if (string_equal (type, "address6"))   { parse_address6   (data, &output->address6_member);   }
	else if (string_equal (type, "interface"))  { parse_interface  (data, &output->interface_member);  }
	else if (string_equal (type, "portnumber")) { parse_portnumber (data, &output->portnumber_member); }
	else if (string_equal (type, "subnet4"))    { parse_subnet4    (data, &output->subnet4_member);    }
	else if (string_equal (type, "subnet6"))    { parse_subnet6    (data, &output->subnet6_member);    }
	else if (string_equal (type, "uint32_t"))   { parse_uint32_t   (data, &output->uint32_t_member);   }
	else { die ("parse_typed (%s, %p): Unrecognized type '%s'", input, output, type); }
}

void parse_mapspec (const char* input, mapspec* output) {
	char* copy _cleanup_free_ = string_duplicate (input);
	char* data = copy;

	char* map = strsep (&data, ",");
	char* key = strsep (&data, ",");
	char* val = strsep (&data, ",");

	if (!map || !key || !val) {
		die ("Invalid map specification: %s", input);
	}

	output->map = string_duplicate (map);
	parse_typed (key, &output->key);
	parse_typed (val, &output->val);
}
