#!/usr/bin/env bats

setup() {
    load ../setup_core
}

@test "test paren: )" {
    assert_minishell_equal_bash ")
echo $?"
}

@test "test paren: ()" {
    assert_minishell_equal_bash "()
echo $?"
}

@test "test paren: )(" {
	assert_minishell_equal_bash ")(
echo $?"
}

@test "test paren: ( () )" {
	assert_minishell_equal_bash "( () )
echo $?"
}

@test "test paren: ( )( )" {
	assert_minishell_equal_bash "( )( )
echo $?"
}

@test "test paren: ( ) ( )" {
	assert_minishell_equal_bash "( ) ( )
echo $?"
}

@test "test paren: ls )" {
    assert_minishell_equal_bash " ls )
echo $?"
}

@test "test paren: ( ls )( ls )" {
	assert_minishell_equal_bash "( ls )( ls )
echo $?"
}

@test "test paren: ( ls ) ( ls )" {
	assert_minishell_equal_bash "( ls ) ( ls )
echo $?"
}

@test "test paren: (ls)" {
    assert_minishell_equal_bash "(ls)
echo $?"
}

@test "test paren: (ls pwd)" {
    assert_minishell_equal_bash "(ls pwd)
echo $?"
}

@test "test paren: (ls && echo ok)" {
    assert_minishell_equal_bash "(ls && echo ok)
echo $?"
}

@test "test paren: (false && echo ok)" {
    assert_minishell_equal_bash "(false && echo ok)
echo $?"
}

@test "test paren: (false || echo ok)" {
    assert_minishell_equal_bash "(false || echo ok)
echo $?"
}

@test "test paren: (true || echo ok)" {
    assert_minishell_equal_bash "(true || echo ok)
echo $?"
}

@test "test paren: ((true) && echo ok)" {
	assert_minishell_equal_bash "((true) && echo ok)
echo $?"
}

@test "test paren: ((false) && echo ok)" {
	assert_minishell_equal_bash "((false) && echo ok)
echo $?"
}

@test "test paren: ((true) || printf ok\n)" {
	assert_minishell_equal_bash "((true) || printf 'ok\n')
echo $?"
}

@test "test paren: ((false) || printf ok\n)" {
	assert_minishell_equal_bash "((false) || printf 'ok\n')
echo $?"
}

@test "test paren: (true && echo ok) | cd .." {
    assert_minishell_equal_bash "(true && echo 'ok') | cd ..
echo $?"
}

@test "test paren: (false && echo ok) | cd .." {
    assert_minishell_equal_bash "(true && echo 'ok') | cd ..
echo $?"
}

@test "test paren: (ls | wc -l) | (cat)" {
    assert_minishell_equal_bash "(true && echo 'ok') | cd ..
echo $?"
}

@test "test paren: (ls && echo ok) | cat" {
    assert_minishell_equal_bash "(true && echo 'ok') | cd ..
echo $?"
}

@test "test paren: (false && echo ok) | cat" {
    assert_minishell_equal_bash "(true && echo 'ok') | cd ..
echo $?"
}

@test "test paren: (false || echo ok) | cat" {
    assert_minishell_equal_bash "(true && echo 'ok') | cd ..
echo $?"
}

@test "test paren: (true || echo ok) | cat" {
    assert_minishell_equal_bash "(true && echo 'ok') | cd ..
echo $?"
}

@test "test paren: ls && (echo ok | cat)" {
    assert_minishell_equal_bash "(true && echo 'ok') | cd ..
echo $?"
}

@test "test paren: (true || (echo ok)) | cat" {
    assert_minishell_equal_bash "(true && echo 'ok') | cd ..
echo $?"
}

@test "test paren: (false && (echo ok)) | cat" {
    assert_minishell_equal_bash "(true && echo 'ok') | cd ..
echo $?"
}

@test "test paren: (ls | cat) | cat" {
    assert_minishell_equal_bash "(ls | cat) | cat
echo $?"
}

@test "test paren: (cd .. && pwd) && pwd" {
    assert_minishell_equal_bash "(cd .. && pwd) && pwd
echo $?"
}

@test "test paren: (echo ') with close paren in single quote str')" {
	assert_minishell_equal_bash "(echo ') with close paren in single quote str')
echo $?"
}

@test "test paren: (echo \") with close paren in doble quote str\")" {
	assert_minishell_equal_bash "(echo \") with close paren in doble quote str\")
echo $?"
}

@test "test paren: (echo '( with open paren in single quote str')" {
	assert_minishell_equal_bash "(echo '( with open paren in single quote str')
echo $?"
}

@test "test paren: (echo \"( with open paren in doble quote str\")" {
	assert_minishell_equal_bash "(echo \"( with open paren in doble quote str\")
echo $?"
}

@test "test paren: (ls | ( ( ( (cat) ) ) ) | ( ( (cat) ) )" {
	assert_minishell_equal_bash "(ls | ( ( ( (cat) ) ) ) ) | ( ( (cat) ) )
echo $?"
}

