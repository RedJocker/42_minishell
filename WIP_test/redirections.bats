#!/usr/bin/env bats
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    redirections.bats                                  :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/26 03:17:46 by dande-je          #+#    #+#              #
#    Updated: 2024/10/26 03:17:46 by dande-je         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

load parallel_helper

@test "input redirection" {
    assert_minishell_equal_bash 'echo hello > output.txt'
}

@test "output redirection with unique files" {
    local test_file="$TEST_CASE_DIR/input.txt"
    echo "test content" > "$test_file"
    assert_minishell_equal_bash "cat < $test_file"
}
