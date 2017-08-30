#pragma once

#ifdef _ALFWRAPPER_NEED_STDINT_H_
#include <stdint.h>
#endif

struct address4 {
	uint32_t raw;
} __attribute__((packed));

struct address6 {
	uint64_t high;
	uint64_t low;
} __attribute__((packed));

struct subnet4 {
	struct address4 address;
	uint8_t prefix;
} __attribute__((packed));

struct subnet6 {
	struct address6 address;
	uint8_t prefix;
} __attribute__((packed));

struct interface {
	uint32_t raw;
} __attribute__((packed));

struct portnumber {
	uint16_t raw;
} __attribute__((packed));

/* Index for BPF_MAP_TYPE_ARRAY, must be exactly four bytes, see bpf(2) */
struct index {
	uint32_t raw;
} __attribute__((packed));
