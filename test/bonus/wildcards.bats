#!/usr/bin/env bats

setup() {
    load ../setup_core
}

@test "test wildcards: ls *" {
    assert_minishell_equal_bash "ls *
printf '$?\n'"
}

@test "test wildcards: ls *.txt" {
    assert_minishell_equal_bash "ls *.txt
printf '$?\n'"
}

@test "test wildcards: cd test && ls *" {
    assert_minishell_equal_bash "mkdir -p test
cd test && ls *
printf '$?\n'"
}

@test "test wildcards: cd test && ls *.txt" {
    assert_minishell_equal_bash "mkdir -p test
cd test && ls *.txt
printf '$?\n'"
}

@test "test wildcards: echo *" {
    assert_minishell_equal_bash "echo *
printf '$?\n'"
}

@test "test wildcards: echo *.txt" {
    assert_minishell_equal_bash "echo *.txt
printf '$?\n'"
}

@test "test wildcards: cd test && echo *" {
    assert_minishell_equal_bash "mkdir -p test
cd test && echo *
printf '$?\n'"
}

@test "test wildcards: cd test && echo *.txt" {
    assert_minishell_equal_bash "mkdir -p test
cd test && echo *.txt
printf '$?\n'"
}

@test "test wildcard(with pattern): echo  *.txt" {
	assert_minishell_equal_bash "touch abc.txt abc.t.txt abc.txt.txt abc.tx def.txt
echo *.txt
printf '$?\n'"
}

@test "test wildcard(with pattern): echo abc*" {
	assert_minishell_equal_bash "touch abc.txt abc.t.txt abc.txt.txt abc.tx def.txt
echo abc*
printf '$?\n'"
}

@test "test wildcard(with pattern): echo *.not_found" {
	assert_minishell_equal_bash "touch abc.txt abc.t.txt abc.txt.txt abc.tx def.txt
echo *.not_found
printf '$?\n'"
}

@test "test wildcard: echo a*.txt" {
	assert_minishell_equal_bash "touch abc.txt abc.t.txt abc.txt.txt abc.tx def.txt
echo *.txt
printf '$?\n'"
}

@test "test wildcards(ordering): echo *" {
    assert_minishell_equal_bash "touch xxx#ab xxx%ab xxx.ab xxxcd xxx#ef xxx%ef xxx.ef
ls -H
echo *
printf '$?\n'"
}

@test "test wildcards(ordering 2): echo *" {
    assert_minishell_equal_bash "touch ABC ABc AbC aBC Abc aBc abC abc
ls -H
echo *
printf '$?\n'"
}

@test "test wildcards: ls > *" {
    assert_minishell_equal_bash "touch a b c
ls > *
printf '$?\n'"
}

@test "test wildcards: echo > *" {
    assert_minishell_equal_bash "touch a b c
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
    assert_minishell_equal_bash "touch a b c
ls > '*'
printf '$?\n'"
}

@test "test wildcards: echo > '*'" {
    assert_minishell_equal_bash "touch a b c
echo > '*'
printf '$?\n'"
}


@test "test wildcard: echo *.txt" {
    assert_minishell_equal_bash "echo *.txt
printf '$?\n'"
}

@test "test wildcard: echo empty_star > *" {
    assert_minishell_equal_bash "echo empty_star > *
printf '$?\n'"
}

@test "test wildcard: echo write_to_a.txt > *" {
    assert_minishell_equal_bash "touch a.txt
echo write_to_a.txt > *
printf '$?\n'
cat a.txt"
}

@test "test wildcard: echo ambiguous > *" {
    assert_minishell_equal_bash "touch a.txt b.txt
echo ambiguous > *
printf '$?\n'
cat a.txt
cat b.txt"
}

@test "test wildcard: echo b_write > b*" {
    assert_minishell_equal_bash "touch a.txt b.txt
echo b_write > b*
printf '$?\n'
cat a.txt
cat b.txt"
}

@test "test wildcard: echo txt_write > *txt" {
    assert_minishell_equal_bash "touch a.java b.txt
echo txt_write > *txt
printf '$?\n'
cat a.java
cat b.txt"
}

