#!/bin/sh -eu

./alfwrapper \
	--set ports,0,1234 \
	--set ports,1,1235 \
	--set ports,2,1236 \
	ebpf/knocking.c \
	true
