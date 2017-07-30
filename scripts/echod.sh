#!/bin/sh -eu

# This script simulates an echo daemon using systemd socket activation

exec systemd-socket-activate --accept --inetd cat
