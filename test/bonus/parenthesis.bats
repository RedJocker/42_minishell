#!/usr/bin/env bats

setup() {
    load ../setup_core
}

@test "test paren invalid: )" {
    assert_minishell_equal_bash ")
printf '$?\n'"
}

@test "test paren invalid: ()" {
    assert_minishell_equal_bash "()
printf '$?\n'"
}

@test "test paren invalid: )(" {
assert_minishell_equal_bash ")(
printf '$?\n'"
}

@test "test paren invalid: ( () )" {
assert_minishell_equal_bash "( () )
printf '$?\n'"
}

@test "test paren invalid: ( )( )" {
assert_minishell_equal_bash "( )( )
printf '$?\n'"
}

@test "test paren invalid: ( ) ( )" {
assert_minishell_equal_bash "( ) ( )
printf '$?\n'"
}

@test "test paren invalid: ls )" {
    assert_minishell_equal_bash " ls )
printf '$?\n'"
}

@test "test paren invalid: ( ls )( ls )" {
assert_minishell_equal_bash "( ls )( ls )
printf '$?\n'"
}

@test "test paren invalid: ( ls ) ( ls )" {
assert_minishell_equal_bash "( ls ) ( ls )
printf '$?\n'"
}

@test "test paren: (ls)" {
    assert_minishell_equal_bash "(ls)
printf '$?\n'"
}

@test "test paren: (ls pwd)" {
    assert_minishell_equal_bash "(ls pwd)
printf '$?\n'"
}

@test "test paren: (ls && printf ok\n)" {
    assert_minishell_equal_bash "(ls && printf 'ok\n')
printf '$?\n'"
}

@test "test paren: (false && printf ok\n)" {
    assert_minishell_equal_bash "(false && printf 'ok\n')
printf '$?\n'"
}

@test "test paren: (false || printf ok\n)" {
    assert_minishell_equal_bash "(false || printf 'ok\n')
printf '$?\n'"
}

@test "test paren: (true || printf ok\n)" {
    assert_minishell_equal_bash "(true || printf 'ok\n')
printf '$?\n'"
}

@test "test paren: ((true) && printf ok\n)" {
	assert_minishell_equal_bash "((true) && printf 'ok\n')
printf '$?\n'"
}

@test "test paren: ((false) && printf ok\n)" {
	assert_minishell_equal_bash "((false) && printf 'ok\n')
printf '$?\n'"
}

@test "test paren: ((true) || printf ok\n)" {
	assert_minishell_equal_bash "((true) || printf 'ok\n')
printf '$?\n'"
}

@test "test paren: ((false) || printf ok\n)" {
	assert_minishell_equal_bash "((false) || printf 'ok\n')
printf '$?\n'"
}

@test "test paren: (ls && echo ok)" {
    assert_minishell_equal_bash "(ls && echo 'ok')
printf '$?\n'"
}

@test "test paren: (false && echo ok)" {
    assert_minishell_equal_bash "(false && echo 'ok')
printf '$?\n'"
}

@test "test paren: (false || echo ok)" {
    assert_minishell_equal_bash "(false || echo 'ok')
printf '$?\n'"
}

@test "test paren: (true || echo ok)" {
    assert_minishell_equal_bash "(false || echo 'ok')
printf '$?\n'"
}

@test "test paren: (true && echo ok) | cd .." {
    assert_minishell_equal_bash "(true && echo 'ok') | cd ..
printf '$?\n'"
}

@test "test paren: (false && echo ok) | cd .." {
    assert_minishell_equal_bash "(true && echo 'ok') | cd ..
printf '$?\n'"
}

@test "test paren: (ls | wc -l) | (cat)" {
    assert_minishell_equal_bash "(true && echo 'ok') | cd ..
printf '$?\n'"
}

@test "test paren: (ls && echo ok) | cat" {
    assert_minishell_equal_bash "(true && echo 'ok') | cd ..
printf '$?\n'"
}

@test "test paren: (false && echo ok) | cat" {
    assert_minishell_equal_bash "(true && echo 'ok') | cd ..
printf '$?\n'"
}

@test "test paren: (false || echo ok) | cat" {
    assert_minishell_equal_bash "(true && echo 'ok') | cd ..
printf '$?\n'"
}

@test "test paren: (true || echo ok) | cat" {
    assert_minishell_equal_bash "(true && echo 'ok') | cd ..
printf '$?\n'"
}

@test "test paren: ls && (echo ok | cat)" {
    assert_minishell_equal_bash "(true && echo 'ok') | cd ..
printf '$?\n'"
}

@test "test paren: (true || (echo ok)) | cat" {
    assert_minishell_equal_bash "(true && echo 'ok') | cd ..
printf '$?\n'"
}

@test "test paren: (false && (echo ok)) | cat" {
    assert_minishell_equal_bash "(true && echo 'ok') | cd ..
printf '$?\n'"
}

@test "test paren: (ls | cat) | cat" {
    assert_minishell_equal_bash "(ls | cat) | cat
printf '$?\n'"
}

@test "test paren: (cd .. && pwd) && pwd" {
    assert_minishell_equal_bash "(cd .. && pwd) && pwd
printf '$?\n'"
}

@test "test paren: (echo ') with close paren in single quote str')" {
	assert_minishell_equal_bash "(echo ') with close paren in single quote str')
printf '$?\n'"
}

@test "test paren: (echo \") with close paren in doble quote str\")" {
	assert_minishell_equal_bash "(echo \") with close paren in doble quote str\")
printf '$?\n'"
}

@test "test paren: (echo '( with open paren in single quote str')" {
	assert_minishell_equal_bash "(echo '( with open paren in single quote str')
printf '$?\n'"
}

@test "test paren: (echo \"( with open paren in doble quote str\")" {
	assert_minishell_equal_bash "(echo \"( with open paren in doble quote str\")
printf '$?\n'"
}

@test "test paren: (ls | ( ( ( (cat) ) ) ) | ( ( (cat) ) )" {
	assert_minishell_equal_bash "(ls | ( ( ( (cat) ) ) ) ) | ( ( (cat) ) )
printf '$?\n'"
}

