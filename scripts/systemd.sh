#!/bin/sh -eu

# This scripts simulates systemd's socket activation as done by PID1

exec systemd-socket-activate --listen '[::]:4242' "${@}"
