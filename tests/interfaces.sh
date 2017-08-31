#!/bin/sh -eu

./alfwrapper \
	--set interfaces,0,lo \
	ebpf/interfaces.c \
	true
