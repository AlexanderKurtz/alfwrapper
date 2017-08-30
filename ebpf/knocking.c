#include "common.h"

BPF_TABLE ("hash", struct index,    struct portnumber, ports,     64);
BPF_TABLE ("hash", struct address6, struct index,      database6, 64);

uint32_t filter (struct __sk_buff *skb) {
	uint32_t length    = skb->len;
	uint32_t interface = skb->ingress_ifindex;
	uint16_t protocol  = bpf_ntohs (skb->protocol);

	struct ethernet_t* ethernet = (void*) bpf_ll_off;
	struct tcp_t*      tcp      = (void*) 0;

	if (protocol == protocol_ip6) {
		struct ip6_t* ip6 = (void*) bpf_net_off;
		struct address6 address;
		address.high = ip6->src_hi;
		address.low = ip6->src_lo;

		struct index index = { .raw = 0 };
		struct index* index_stored = database6.lookup (&address);

		if (index_stored) {
			index = *index_stored;
		}

		struct portnumber* expected_port = ports.lookup (&index);

		if (expected_port) {
			if (tcp->src_port == expected_port->raw) {
				index.raw++;
				database6.update (&address, &index);
			}
		} else {
			KEEP();
		}
	}

	DROP();
}
