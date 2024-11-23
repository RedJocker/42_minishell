#!/bin/bash

# Clean old test.
rm -rf test/output_error > /dev/null || true
rm -rf test/temp > /dev/null || true
rm -rf test/logs > /dev/null || true

# Default values
THREADS=$(($(nproc) - 1))
USE_VALGRIND=1
USE_THREADS=0

# Function to show usage
show_usage() {
    echo "Usage: $0 [-v] [-t [threads]]"
    echo "  -v: Run tests without valgrind"
    echo "  -t: Enable threads (optional: specify number of threads, default: $THREADS)"
    exit 1
}

# Process each argument manually
while [ $# -gt 0 ]; do
    case "$1" in
        -v)
            USE_VALGRIND=0
            shift
            ;;
        -t)
            USE_THREADS=1
            if [ -n "$2" ] && [ "${2:0:1}" != "-" ]; then
                THREADS=$2
                shift
            fi
            shift
            ;;
        *)
            show_usage
            ;;
    esac
done

echo "VALGRIND = $([ $USE_VALGRIND -eq 1 ] && echo "enabled" || echo "disabled")"

# Export the valgrind flag so it's available to all BATS tests
export USE_VALGRIND

if [ "$USE_THREADS" -eq 1 ]; then
	echo "Starting test with ${THREADS} threads"
	bats --jobs "${THREADS}" test/*/*.bats
else
	echo "Starting test without threads"
	bats  test/*/*.bats
fi

# Clean folder.
rm -rf test/temp > /dev/null || true
