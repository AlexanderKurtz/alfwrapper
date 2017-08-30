#!/bin/sh -eu

./alfwrapper \
	--address "${1-::}" \
	--port "${2-4242}" \
	--set 'ports,uint8_t:0,portnumber:15000' \
	--set 'ports,uint8_t:1,portnumber:15001' \
	--set 'ports,uint8_t:2,portnumber:15002' \
	'ebpf/knocking.c' \
	'scripts/echod.sh'
