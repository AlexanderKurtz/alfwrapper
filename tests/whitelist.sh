#!/bin/sh -eu

./alfwrapper \
	--set allow4,index:0,subnet4:127.0.0.1/24 \
	--set allow6,index:0,subnet6:::1/128 \
	ebpf/whitelist.c \
	true
