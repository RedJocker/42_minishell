#!/usr/bin/env bats

load ../setup_core

@test "pwd: pwd" {
    assert_minishell_equal_bash "pwd
printf 'Status code: $?\n'"
}

@test "pwd: multiple calls" {
    assert_minishell_equal_bash "pwd
printf 'Status code: $?\n'
pwd
printf 'Status code: $?\n'
pwd
printf 'Status code: $?\n'"
}

@test "pwd: with cd" {
    assert_minishell_equal_bash "pwd
printf 'Status code: $?\n'
cd /tmp
pwd
printf 'Status code: $?\n'"
}

@test "pwd: with option -P" {
    assert_minishell_equal_bash "pwd -P
printf 'Status code: $?\n'"
}

@test "pwd: with option -L" {
    assert_minishell_equal_bash "pwd -L
printf 'Status code: $?\n'"
}

@test "pwd: in subshell" {
    assert_minishell_equal_bash "(pwd)
printf 'Status code: $?\n'"
}

@test "pwd: with pipes" {
    assert_minishell_equal_bash "pwd | cat
printf 'Status code: $?\n'"
}

@test "pwd: with redirection" {
    assert_minishell_equal_bash "pwd > output.txt
printf 'Status code: $?\n'
cat output.txt"
}
