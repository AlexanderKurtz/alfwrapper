#pragma once

typedef uint32_t address4;

typedef struct __attribute__((packed)) {
	uint64_t high;
	uint64_t low;
} address6;

typedef struct __attribute__((packed)) {
	address4 address;
	uint8_t prefix;
} subnet4;

typedef struct __attribute__((packed)) {
	address6 address;
	uint8_t prefix;
} subnet6;

typedef uint32_t interface;

typedef uint16_t portnumber;

typedef int socktype;
