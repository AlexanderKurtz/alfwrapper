#include "common.h"

#define MAX_SUBNETS (20)

BPF_TABLE ("hash", struct index, struct subnet6, allow6, MAX_SUBNETS);

uint32_t filter (struct __sk_buff *skb) {
	uint16_t protocol = bpf_ntohs (skb->protocol);

	if (protocol == protocol_ip6) {
		struct ip6_t* ip6 = (void*) bpf_net_off;
		#pragma unroll
		for (int i = 0; i < MAX_SUBNETS; i++) {
			struct index index = { .raw = i };
			struct subnet6* current = allow6.lookup (&index);
			if (current) {
				if (match_subnet6 (current, ip6->src_hi, ip6->src_lo)) {
					KEEP();
				}
			} else {
				break;
			}
		}
	}

	DROP();
}
