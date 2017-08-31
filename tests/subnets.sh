#!/bin/sh -eu

./alfwrapper \
	--set allow6,0,::1/128 \
	ebpf/subnets.c \
	true
