#pragma once

#include <bcc/helpers.h>
#include <bcc/proto.h>

#include "data.h"

#define FUNC static inline __attribute__((always_inline))

#define DROP() { return 0; };
#define KEEP() { return skb->len; };

/* From /usr/include/linux/if_ether.h */
const uint16_t protocol_ip6 = 0x86DD;

/* From /usr/include/linux/filter.h */
const size_t bpf_net_off = -0x100000;
const size_t bpf_ll_off  = -0x200000;

FUNC bool match_subnet6 (const struct subnet6* subnet, uint64_t high, uint64_t low) {
	if (subnet->prefix <= 64) {
		int drop = 64 - subnet->prefix;
		return (high >> drop) == (subnet->address.high >> drop);
	} else {
		int drop = 128 - subnet->prefix;
		return high == subnet->address.high && (low >> drop) == (subnet->address.low >> drop);
	}
}
