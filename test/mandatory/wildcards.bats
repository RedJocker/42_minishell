#!/usr/bin/env bats

load ../setup_core

@test "test wildcards: ls *" {
    assert_minishell_equal_bash "ls *
echo \$?"
}

@test "test wildcards: ls *.txt" {
    assert_minishell_equal_bash "ls *.txt
echo \$?"
}

@test "test wildcards: cd test && ls *" {
    assert_minishell_equal_bash "mkdir -p test
cd test && ls *
echo \$?"
}

@test "test wildcards: cd test && ls *.txt" {
    assert_minishell_equal_bash "mkdir -p test
cd test && ls *.txt
echo \$?"
}

@test "test wildcards: echo *" {
    assert_minishell_equal_bash "echo *
echo \$?"
}

@test "test wildcards: echo *.txt" {
    assert_minishell_equal_bash "echo *.txt
echo \$?"
}

@test "test wildcards: cd test && echo *" {
    assert_minishell_equal_bash "mkdir -p test
cd test && echo *
echo \$?"
}

@test "test wildcards: cd test && echo *.txt" {
    assert_minishell_equal_bash "mkdir -p test
cd test && echo *.txt
echo \$?"
}

@test "test wildcard(with pattern): echo *.txt" {
	assert_minishell_equal_bash "touch abc.txt abc.t.txt abc.txt.txt abc.tx def.txt
echo *.txt
echo \$?"
}

@test "test wildcard(with pattern): echo abc*" {
	assert_minishell_equal_bash "touch abc.txt abc.t.txt abc.txt.txt abc.tx def.txt
echo abc*
echo \$?"
}

@test "test wildcard(with pattern): echo *.not_found" {
	assert_minishell_equal_bash "touch abc.txt abc.t.txt abc.txt.txt abc.tx def.txt
echo *.not_found
echo \$?"
}

@test "test wildcard: echo a*.txt" {
	assert_minishell_equal_bash "touch abc.txt abc.t.txt abc.txt.txt abc.tx def.txt
echo *.txt
echo \$?"
}

@test "test wildcards(ordering): echo *" {
    assert_minishell_equal_bash "touch xxx#ab xxx%ab xxx.ab xxxcd xxx#ef xxx%ef xxx.ef
ls -H
echo *
echo \$?"
}

@test "test wildcards(ordering 2): echo *" {
    assert_minishell_equal_bash "touch ABC ABc AbC aBC Abc aBc abC abc
ls -H
echo *
echo \$?"
}

@test "test wildcards: ls > *" {
    assert_minishell_equal_bash "touch a b c
ls > *
echo \$?"
}

@test "test wildcards: echo > *" {
    assert_minishell_equal_bash "touch a b c
echo > *
echo \$?"
}

@test "test wildcards: ls \"*\"" {
    assert_minishell_equal_bash "ls \"*\"
echo \$?"
}

@test "test wildcards: echo \"*\"" {
    assert_minishell_equal_bash "
echo \"*\"
echo \$?"
}

@test "test wildcards: ls > '*'" {
    assert_minishell_equal_bash "touch a b c
ls > '*'
echo \$?"
}

@test "test wildcards: echo > '*'" {
    assert_minishell_equal_bash "touch a b c
echo > '*'
echo \$?"
}


@test "test wildcard: echo *.txt" {
    assert_minishell_equal_bash "echo *.txt
echo \$?"
}

@test "test wildcard: echo empty_star > *" {
    assert_minishell_equal_bash "echo empty_star > *
echo \$?"
}

@test "test wildcard: echo write_to_a.txt > *" {
    assert_minishell_equal_bash "touch a.txt
echo write_to_a.txt > *
echo \$?
cat a.txt"
}

@test "test wildcard: echo ambiguous > *" {
    assert_minishell_equal_bash "touch a.txt b.txt
echo ambiguous > *
echo \$?
cat a.txt
cat b.txt"
}

@test "test wildcard: echo b_write > b*" {
    assert_minishell_equal_bash "touch a.txt b.txt
echo b_write > b*
echo \$?
cat a.txt
cat b.txt"
}

@test "test wildcard: echo txt_write > *txt" {
    assert_minishell_equal_bash "touch a.java b.txt
echo txt_write > *txt
echo \$?
cat a.java
cat b.txt"
}

@test "test wildcard: echo \"*\"a\"*\"a" {
    assert_minishell_equal_bash "touch a aa aaa
echo *a*a
echo \$?"
}

@test "test wildcard: echo a\"*\"a" {
    assert_minishell_equal_bash "touch a aa aaa
echo \"*\"a
echo \$?"
}
