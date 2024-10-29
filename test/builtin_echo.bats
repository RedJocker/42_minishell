#!/usr/bin/env bats
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    builtins.bats                                      :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/26 03:16:58 by dande-je          #+#    #+#              #
#    Updated: 2024/10/26 03:16:58 by dande-je         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

setup() {
    load parallel_helper
}

@test "echo \"Hello World\"" {
    assert_minishell_equal_bash "echo \"Hello World\""
}

@test "echo arg1" {
    assert_minishell_equal_bash "echo arg1"
}

@test "echo arg1 arg2 arg3" {
    assert_minishell_equal_bash "echo arg1 arg2 arg3"
}

@test "echo working > a.txt" {
    assert_minishell_equal_bash "ls $TEST_CASE_DIR
cat "file a" > $TEST_CASE_DIR/a.txt
echo working > $TEST_CASE_DIR/a.txt
cat $TEST_CASE_DIR/a.txt
ls $TEST_CASE_DIR"
}

@test "echo > a.txt working" {
    assert_minishell_equal_bash "ls $TEST_CASE_DIR
cat "file a" > $TEST_CASE_DIR/a.txt
echo > $TEST_CASE_DIR/a.txt working
cat $TEST_CASE_DIR/a.txt
ls $TEST_CASE_DIR"
}

@test "> a.txt echo working" {
    assert_minishell_equal_bash "ls $TEST_CASE_DIR
cat "file a" > $TEST_CASE_DIR/a.txt
> $TEST_CASE_DIR/a.txt echo working
cat $TEST_CASE_DIR/a.txt
ls $TEST_CASE_DIR"
}
