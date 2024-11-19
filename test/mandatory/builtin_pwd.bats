#!/usr/bin/env bats

load ../setup_core

@test "test pwd: pwd" {
    assert_minishell_equal_bash "pwd
printf '$?\n'"
}

@test "test pwd: with exit status" {
    assert_minishell_equal_bash "pwd
echo \$?"
}

@test "test pwd: multiple calls" {
    assert_minishell_equal_bash "pwd
pwd
pwd"
}

@test "test pwd: with cd" {
    assert_minishell_equal_bash "pwd
cd /tmp
pwd"
}

@test "test pwd: with option -P" {
    assert_minishell_equal_bash "pwd -P"
}

@test "test pwd: with option -L" {
    assert_minishell_equal_bash "pwd -L"
}

@test "test pwd: in subshell" {
    assert_minishell_equal_bash "(pwd)"
}

@test "test pwd: with pipes" {
    assert_minishell_equal_bash "pwd | cat"
}

@test "test pwd: with redirection" {
    assert_minishell_equal_bash "pwd > output.txt
cat output.txt"
}
