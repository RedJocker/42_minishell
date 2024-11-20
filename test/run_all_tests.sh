#!/bin/bash

# Clean old test.
rm -rf test/output_error > /dev/null || true
rm -rf test/temp > /dev/null || true
rm -rf test/logs > /dev/null || true
# Test serially.
THREADS=$(($(nproc) - 1))
echo "THREADS = ${THREADS}"
bats --jobs $THREADS test/*/*.bats
# Clean folder.
rm -rf test/temp > /dev/null || true
