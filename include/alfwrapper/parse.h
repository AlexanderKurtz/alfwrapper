#pragma once

#include <alfwrapper/data.h>    // for address4, address6, index, interface
#include <alfwrapper/socket.h>  // for socktype

union key_value_type {
	struct address4   address4_member;
	struct address6   address6_member;
	struct index      index_member;
	struct interface  interface_member;
	struct portnumber portnumber_member;
	struct subnet4    subnet4_member;
	struct subnet6    subnet6_member;
};

struct parameter {
	const char* table;
	const char* key;
	const char* value;
};

void parse_typed (const char* type, const char* data, union key_value_type* output);
void parse_socktype (const char* input, socktype* output);
void parse_parameter (const char* input, struct parameter* output);
