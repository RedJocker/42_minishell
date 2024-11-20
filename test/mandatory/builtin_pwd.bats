#!/usr/bin/env bats

load ../setup_core_without_valgrind

@test "pwd: pwd" {
    assert_minishell_equal_bash "pwd"
}

@test "pwd: with exit status" {
    assert_minishell_equal_bash "
pwd
echo \$?
"
}

@test "pwd: multiple calls" {
    assert_minishell_equal_bash "
pwd
pwd
pwd
"
}

@test "pwd: with cd" {
    assert_minishell_equal_bash "
pwd
cd /tmp
pwd
"
}

@test "pwd: with option -P" {
    assert_minishell_equal_bash "
pwd -P
"
}

@test "pwd: with option -L" {
    assert_minishell_equal_bash "
pwd -L
"
}

@test "pwd: in subshell" {
    assert_minishell_equal_bash "
(pwd)
"
}

@test "pwd: with pipes" {
    assert_minishell_equal_bash "
pwd | cat
"
}

@test "pwd: with redirection" {
    assert_minishell_equal_bash "
pwd > output.txt
cat output.txt
"
}
