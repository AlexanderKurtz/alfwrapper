#!/bin/sh -eu

echo "Knocking 1/3"
ncat --source-port 15000 --wait='1' "${1-::1}" "${2-4242}" < /dev/null > /dev/null || true

echo "Knocking 2/3"
ncat --source-port 15001 --wait='1' "${1-::1}" "${2-4242}" < /dev/null > /dev/null || true

echo "Knocking 3/3"
ncat --source-port 15002 --wait='1' "${1-::1}" "${2-4242}" < /dev/null > /dev/null || true

echo "You should be whitelisted now, trying simple echo!"
echo foobar | ncat "${1-::1}" "${2-4242}"
