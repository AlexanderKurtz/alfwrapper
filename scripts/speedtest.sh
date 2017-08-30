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
	--set 'allow4,00,1.0.0.0/24' \
	--set 'allow4,01,1.0.0.1/24' \
	--set 'allow4,02,1.0.0.2/24' \
	--set 'allow4,03,1.0.0.3/24' \
	--set 'allow4,04,1.0.0.4/24' \
	--set 'allow4,05,1.0.0.5/24' \
	--set 'allow4,06,1.0.0.6/24' \
	--set 'allow4,07,1.0.0.7/24' \
	--set 'allow4,08,1.0.0.8/24' \
	--set 'allow4,09,1.0.0.9/24' \
	--set 'allow4,10,1.0.0.10/24' \
	--set 'allow4,11,1.0.0.11/24' \
	--set 'allow4,12,1.0.0.12/24' \
	--set 'allow4,13,1.0.0.13/24' \
	--set 'allow4,14,127.0.0.1/24' \
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
