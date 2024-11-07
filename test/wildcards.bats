#!/usr/bin/env bats

setup() {
    load parallel_helper
}

@test "test wildcards: ls *" {
    assert_minishell_equal_bash "ls *
printf '$?\n'"
}

@test "test wildcards: echo *" {
    assert_minishell_equal_bash "echo *
printf '$?\n'"
}

@test "test wildcards ordering: touch xxxx#ab xxx%ab xxx.ab xxxcd xxx#ef xxx%ef xxx.ef\n echo *" {
    assert_minishell_equal_bash "cd $TEST_CASE_DIR
touch xxx#ab xxx%ab xxx.ab xxxcd xxx#ef xxx%ef xxx.ef
ls -H
echo *
printf '$?\n'"
}

@test "test wildcards ordering: touch ABC ABc AbC aBC Abc aBc abC abc \n echo *" {
    assert_minishell_equal_bash "cd $TEST_CASE_DIR
touch ABC ABc AbC aBC Abc aBc abC abc
ls -H
echo *
printf '$?\n'"
}

@test "test wildcards: ls > *" {
    assert_minishell_equal_bash "cd $TEST_CASE_DIR
touch a b c
ls > *
printf '$?\n'"
}

@test "test wildcards: echo > *" {
    assert_minishell_equal_bash "cd $TEST_CASE_DIR
touch a b c
echo > *
printf '$?\n'"
}

@test "test wildcards: ls \"*\"" {
    assert_minishell_equal_bash "ls \"*\"
printf '$?\n'"
}

@test "test wildcards: echo \"*\"" {
    assert_minishell_equal_bash "
echo \"*\"
printf '$?\n'"
}

@test "test wildcards: ls > '*'" {
    assert_minishell_equal_bash "cd $TEST_CASE_DIR
touch a b c
ls > '*'
printf '$?\n'"
}

@test "test wildcards: echo > '*'" {
    assert_minishell_equal_bash "cd $TEST_CASE_DIR
touch a b c
echo > '*'
printf '$?\n'"
}


@test "test wildcard: echo *.txt" {
    
    assert_minishell_equal_bash "echo *.txt
"
}

