#include "common.h"

#define MAX_INTERFACES (16)

BPF_TABLE ("hash", struct index, struct interface, interfaces, MAX_INTERFACES);

uint32_t filter (struct __sk_buff *skb) {
	uint32_t interface = skb->ingress_ifindex;

	#pragma unroll
	for (int i = 0; i < MAX_INTERFACES; i++) {
		struct index index = { .raw = i };
		struct interface* current = interfaces.lookup (&index);
		if (current) {
			if (current->raw == interface) {
				KEEP();
			}
		} else {
			break;
		}
	}

	DROP();
}
