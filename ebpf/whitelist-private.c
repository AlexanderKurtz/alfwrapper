#include "common.h"

static const subnet4 allow4[] =
	{ { 0x0a000000,  8 }
	, { 0x7f000000, 24 }
	, { 0xac0a0000, 12 }
	, { 0xc0a80000, 16 }
	};

static const subnet6 allow6[] =
	{ { 0x00000000, 0x00000001, 128 }
	, { 0xfd000000, 0x00000000,   8 }
	};

uint32_t filter (struct __sk_buff *skb) {
	uint32_t length    = skb->len;
	uint32_t interface = skb->ingress_ifindex;
	uint16_t protocol  = bpf_ntohs (skb->protocol);

	struct ethernet_t* ethernet = (void*) bpf_ll_off;
	struct tcp_t*      tcp      = (void*) 0;

	if (protocol == protocol_ip4) {
		struct ip_t* ip4 = (void*) bpf_net_off;
		#pragma unroll
		for (int i = 0; i < NUM (allow4); i++) {
			if (match_subnet4 (allow4 + i, ip4->src)) {
				KEEP();
			}
		}
	} else if (protocol == protocol_ip6) {
		struct ip6_t* ip6 = (void*) bpf_net_off;
		#pragma unroll
		for (int i = 0; i < NUM (allow6); i++) {
			if (match_subnet6 (allow6 + i, ip6->src_hi, ip6->src_lo)) {
				KEEP();
			}
		}
	}

	DROP();
}
