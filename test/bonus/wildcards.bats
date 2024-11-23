#!/usr/bin/env bats

load ../setup_core

@test "wildcards: ls *" {
    assert_minishell_equal_bash "ls *
echo Status code: $?"
}

@test "wildcards: ls *.txt" {
    assert_minishell_equal_bash "ls *.txt
echo Status code: $?"
}

@test "wildcards: cd test && ls *" {
    assert_minishell_equal_bash "mkdir -p test
cd test && ls *
echo Status code: $?"
}

@test "wildcards: cd test && ls *.txt" {
    assert_minishell_equal_bash "mkdir -p test
cd test && ls *.txt
echo Status code: $?"
}

@test "wildcards: cd test && echo *" {
    assert_minishell_equal_bash "mkdir -p test
cd test && echo *
echo Status code: $?"
}

@test "wildcards: cd test && echo *.txt" {
    assert_minishell_equal_bash "mkdir -p test
cd test && echo *.txt
echo Status code: $?"
}

@test "wildcards: echo *.txt" {
    assert_minishell_equal_bash "echo *.txt
echo Status code: $?"
}

@test "wildcards: echo *.txt (v2)" {
	assert_minishell_equal_bash "touch abc.txt abc.t.txt abc.txt.txt abc.tx def.txt
echo *.txt
echo Status code: $?"
}

@test "wildcards: echo abc*" {
	assert_minishell_equal_bash "touch abc.txt abc.t.txt abc.txt.txt abc.tx def.txt
echo abc*
echo Status code: $?"
}

@test "wildcards: echo *.not_found" {
	assert_minishell_equal_bash "touch abc.txt abc.t.txt abc.txt.txt abc.tx def.txt
echo *.not_found
echo Status code: $?"
}

@test "wildcards: echo a*.txt" {
	assert_minishell_equal_bash "touch abc.txt abc.t.txt abc.txt.txt abc.tx def.txt
echo *.txt
echo Status code: $?"
}

@test "wildcards: echo *" {
    assert_minishell_equal_bash "echo *
echo Status code: $?"
}

@test "wildcards: echo * (v2)" {
    assert_minishell_equal_bash "touch xxx#ab xxx%ab xxx.ab xxxcd xxx#ef xxx%ef xxx.ef
echo *
echo Status code: $?"
}

@test "wildcards: echo * (v3)" {
    assert_minishell_equal_bash "touch ABC ABc AbC aBC Abc aBc abC abc
echo *
echo Status code: $?"
}

@test "wildcards: ls > *" {
    assert_minishell_equal_bash "touch a b c
ls > *
echo \$?"
}

@test "wildcards: echo > *" {
    assert_minishell_equal_bash "touch a b c
echo > *
echo Status code: $?"
}

@test "wildcards: ls \"*\"" {
    assert_minishell_equal_bash "ls \"*\"
echo Status code: $?"
}

@test "wildcards: echo \"*\"" {
    assert_minishell_equal_bash "echo \"*\"
echo Status code: $?"
}

@test "wildcards: ls > '*'" {
    assert_minishell_equal_bash "touch a b c
ls > '*'
echo Status code: $?"
}

@test "wildcards: echo > '*'" {
    assert_minishell_equal_bash "touch a b c
echo > '*'
echo Status code: $?"
}

@test "wildcards: echo empty_star > *" {
    assert_minishell_equal_bash "echo empty_star > *
echo Status code: $?"
}

@test "wildcards: echo write_to_a.txt > *" {
    assert_minishell_equal_bash "touch a.txt
echo write_to_a.txt > *
echo \$?
cat a.txt
"
}

@test "wildcards: echo ambiguous > *" {
    assert_minishell_equal_bash "touch a.txt b.txt
echo ambiguous > *
echo \$?
cat a.txt
cat b.txt
"
}

@test "wildcards: echo b_write > b*" {
    assert_minishell_equal_bash "touch a.txt b.txt
echo b_write > b*
echo \$?
cat a.txt
cat b.txt
"
}

@test "wildcards: echo txt_write > *txt" {
    assert_minishell_equal_bash "touch a.java b.txt
echo txt_write > *txt
echo \$?
cat a.java
cat b.txt
"
}

@test "wildcards: echo \"*\"a\"*\"a" {
    assert_minishell_equal_bash "touch a aa aaa
echo *a*a
echo Status code: $?"
}

@test "wildcards: echo a\"*\"a" {
    assert_minishell_equal_bash "touch a aa aaa
echo \"*\"a
echo Status code: $?"
}
