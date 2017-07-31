#pragma once

#include <stdint.h>                  // for uint32_t
#include "../../ebpf/common-data.h"  // for address4, address6, interface

typedef union __attribute__((packed)) {
	address4   address4_member;
	address6   address6_member;
	interface  interface_member;
	portnumber portnumber_member;
	socktype   socktype_member;
	subnet4    subnet4_member;
	subnet6    subnet6_member;
	uint32_t   uint32_t_member;
} key_value_type;

typedef struct {
	const char*    map;
	key_value_type key;
	key_value_type val;
} mapspec;

void parse_mapspec (const char* input, mapspec* output);
void parse_socktype (const char* input, socktype* output);
