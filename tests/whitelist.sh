#!/bin/sh -eu

./alfwrapper \
	--set allow4,0,127.0.0.1/24 \
	--set allow6,0,::1/128 \
	ebpf/whitelist.c \
	true
