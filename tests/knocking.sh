#!/bin/sh -eu

./alfwrapper \
	--set ports,index:0,portnumber:1234 \
	--set ports,index:1,portnumber:1235 \
	--set ports,index:2,portnumber:1236 \
	ebpf/knocking.c \
	true
