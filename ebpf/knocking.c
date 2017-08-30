#include "common.h"

BPF_TABLE ("hash", uint8_t,  portnumber, ports,     64)
BPF_TABLE ("hash", address4, uint8_t,    database4, 64)
BPF_TABLE ("hash", address6, uint8_t,    database6, 64)

uint32_t filter (struct __sk_buff *skb) {
	uint32_t length    = skb->len;
	uint32_t interface = skb->ingress_ifindex;
	uint16_t protocol  = bpf_ntohs (skb->protocol);

	struct ethernet_t* ethernet = (void*) bpf_ll_off;
	struct tcp_t*      tcp      = (void*) 0;

	if (protocol == protocol_ip4) {
		struct ip_t* ip4 = (void*) bpf_net_off;
		address4 address = ip4->src;

		uint8_t* index_stored = database4.lookup (&address);
		uint8_t index = index_stored == NULL ? 0 : *index_stored;

		portnumber* expected_port = ports.lookup (&index);

		if (expected_port) {
			if (tcp->src_port == *expected_port) {
				index++;
				database4.update (&address, &index);
			}
		} else {
			KEEP();
		}
	} else if (protocol == protocol_ip6) {
		struct ip6_t* ip6 = (void*) bpf_net_off;
		address6 address;
		address.high = ip6->src_hi;
		address.low = ip6->src_lo;

		uint8_t* index_stored = database6.lookup (&address);
		uint8_t index = index_stored == NULL ? 0 : *index_stored;

		portnumber* expected_port = ports.lookup (&index);

		if (expected_port) {
			if (tcp->src_port == *expected_port) {
				index++;
				database6.update (&address, &index);
			}
		} else {
			KEEP();
		}
	}

	DROP();
}
