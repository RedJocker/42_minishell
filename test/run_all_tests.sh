#!/bin/bash
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    run_all_tests.sh                                   :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/26 03:14:21 by dande-je          #+#    #+#              #
#    Updated: 2024/10/26 03:14:21 by dande-je         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Clean old test.
rm -f ./*/bash_*.txt > /dev/null || true
rm -f ./*/mini_*.txt > /dev/null || true
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
