#!/bin/bash

# Clean old test.
rm -rf test/output_error > /dev/null || true
rm -rf test/temp > /dev/null || true
rm -rf test/logs > /dev/null || true

# Default values
THREADS=$(($(nproc) - 1))
USE_VALGRIND=0

while getopts "vt:" opt; do
    case $opt in
        v)
            USE_VALGRIND=1
            ;;
        t)
            THREADS=$OPTARG
            ;;
        *)
            echo "Usage: $0 [-v] [-t threads]"
            echo "  -v: Run tests with valgrind"
            echo "  -t: Specify number of threads (default: $THREADS)"
            exit 1
            ;;
    esac
done

echo "VALGRIND = $([ $USE_VALGRIND -eq 1 ] && echo "enabled" || echo "disabled")"

# Export the valgrind flag so it's available to all BATS tests
export USE_VALGRIND

echo "Starting test with ${THREADS} threads"
bats --jobs "${THREADS}" test/*/*.bats

# Clean folder.
rm -rf test/temp > /dev/null || true
