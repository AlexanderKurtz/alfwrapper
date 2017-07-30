#include "common.h"

#define MAX_ALLOW (15)

BPF_TABLE ("array", uint32_t, subnet4, allow4, MAX_ALLOW);
BPF_TABLE ("array", uint32_t, subnet6, allow6, MAX_ALLOW);

uint32_t filter (struct __sk_buff *skb) {
	uint32_t length    = skb->len;
	uint32_t interface = skb->ingress_ifindex;
	uint16_t protocol  = bpf_ntohs (skb->protocol);

	struct ethernet_t* ethernet = (void*) bpf_ll_off;
	struct tcp_t*      tcp      = (void*) 0;

	if (protocol == protocol_ip4) {
		struct ip_t* ip4 = (void*) bpf_net_off;
		#pragma unroll
		for (int i = 0; i < MAX_ALLOW; i++) {
			uint32_t index = i;
			subnet4* current = allow4.lookup (&index);
			if (current && current->prefix) {
				if (match_subnet4 (current, ip4->src)) {
					KEEP();
				}
			} else {
				break;
			}
		}
	} else if (protocol == protocol_ip6) {
		struct ip6_t* ip6 = (void*) bpf_net_off;
		#pragma unroll
		for (int i = 0; i < MAX_ALLOW; i++) {
			uint32_t index = i;
			subnet6* current = allow6.lookup (&index);
			if (current && current->prefix) {
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
