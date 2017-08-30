#!/bin/sh -eu

./alfwrapper \
	--set ports,uint8_t:0,portnumber:1234 \
	--set ports,uint8_t:1,portnumber:1235 \
	--set ports,uint8_t:2,portnumber:1236 \
	ebpf/knocking.c \
	true
