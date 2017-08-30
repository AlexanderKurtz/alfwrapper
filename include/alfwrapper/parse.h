#pragma once

#include <alfwrapper/data.h>    // for address4, address6, index, interface
#include <alfwrapper/socket.h>  // for socktype

typedef union __attribute__((packed)) {
	socktype          socktype_member;
	struct address4   address4_member;
	struct address6   address6_member;
	struct index      index_member;
	struct interface  interface_member;
	struct portnumber portnumber_member;
	struct subnet4    subnet4_member;
	struct subnet6    subnet6_member;
} key_value_type;

typedef struct {
	const char*    map;
	key_value_type key;
	key_value_type val;
} mapspec;

void parse_mapspec (const char* input, mapspec* output);
void parse_socktype (const char* input, socktype* output);
