#!/bin/bash

# Clean old test.
rm -rf test/output_error > /dev/null || true
# Test serially.
bats test/*/*.bats
