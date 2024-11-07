#!/bin/bash

# Clean old test.
rm -rf test/output_error > /dev/null || true
# Test serially.
bats test/*.bats

# Alternative with max system jobs:
# bats --jobs 4 test/*.bats

# bats --jobs 2 test/*.bats --no-parallelize-across-files
# TRYING parallel
# # run_tests.sh
# set -e
#
# # Clean up any existing processes
# pkill -f minishell 2>/dev/null || true
# sleep 1
#
# # Remove old temp directories
# rm -rf ./test/temp_* 2>/dev/null || true
#
# # Run tests in parallel
# bats --jobs $(nproc --ignore=1) WIP_test/*.bats --no-parallelize-across-files
#
# # Cleanup after tests
# pkill -f minishell 2>/dev/null || true
# rm -rf ./test/temp_* 2>/dev/null || true
