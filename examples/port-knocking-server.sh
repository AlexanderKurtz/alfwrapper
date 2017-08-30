#!/bin/sh -eu

./alfwrapper \
	--address "${1-::}" \
	--port "${2-4242}" \
	--set 'ports,0,15000' \
	--set 'ports,1,15001' \
	--set 'ports,2,15002' \
	'ebpf/knocking.c' \
	'scripts/echod.sh'
