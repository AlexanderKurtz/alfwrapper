#!/bin/sh -eu

client()(
	echo
	echo "Reading 1GB of data from /dev/zero through the echo daemon"
	head --bytes='1GB' /dev/zero | ncat ::1 4242 | dd bs=1MiB of=/dev/null
)

# Start the server
echo
echo "Spawning a simple echo daemon on [::1]:4242 *without* aflwrapper"
systemd-socket-activate --listen '[::1]:4242' --accept --inetd cat 1>/dev/null 2>/dev/null & daemon="${!}"

# Wait for the server to become ready
sleep 1

# Run the client
client

# Terminate the server
kill "${daemon}"

# Wait for the server to terminate
sleep 1

# Start the server
echo
echo "Spawning a simple echo daemon on [::1]:4242 *with* aflwrapper with lots of rules"
./alfwrapper \
	--address '::1' \
	--port '4242' \
	\
	--set 'allow6,00,1::0/72' \
	--set 'allow6,01,1::1/72' \
	--set 'allow6,02,1::2/72' \
	--set 'allow6,03,1::3/72' \
	--set 'allow6,04,1::4/72' \
	--set 'allow6,05,1::5/72' \
	--set 'allow6,06,1::6/72' \
	--set 'allow6,07,1::7/72' \
	--set 'allow6,08,1::8/72' \
	--set 'allow6,09,1::9/72' \
	--set 'allow6,10,1::10/72' \
	--set 'allow6,11,1::11/72' \
	--set 'allow6,12,1::12/72' \
	--set 'allow6,13,1::13/72' \
	--set 'allow6,14,::1/128' \
	\
	ebpf/whitelist.c \
	systemd-socket-activate --accept --inetd cat 1>/dev/null 2>/dev/null & daemon="${!}"

# Wait for the server to become ready
sleep 1

# Run the client
client

# Terminate the server
kill "${daemon}"
