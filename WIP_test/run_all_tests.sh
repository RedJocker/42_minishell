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

# Alternative with max system jobs:
# bats --jobs 4 test/*.bats

# Test serially.
bats WIP_test/*.bats
