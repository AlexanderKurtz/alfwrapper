#pragma once

#include <alfwrapper/data.h>    // for address4, address6, interface, portnu...
#include <alfwrapper/socket.h>  // for socktype
#include <stdint.h>             // for uint16_t, uint32_t, uint8_t

typedef union __attribute__((packed)) {
	socktype          socktype_member;
	struct address4   address4_member;
	struct address6   address6_member;
	struct interface  interface_member;
	struct portnumber portnumber_member;
	struct subnet4    subnet4_member;
	struct subnet6    subnet6_member;
	uint16_t          uint16_t_member;
	uint32_t          uint32_t_member;
	uint8_t           uint8_t_member;
} key_value_type;

typedef struct {
	const char*    map;
	key_value_type key;
	key_value_type val;
} mapspec;

void parse_mapspec (const char* input, mapspec* output);
void parse_socktype (const char* input, socktype* output);
