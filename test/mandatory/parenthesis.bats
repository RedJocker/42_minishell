#!/usr/bin/env bats

load ../setup_core

# Basic syntax tests
@test "test paren: )" {
    assert_minishell_equal_bash ")
echo \$?"
}

@test "test paren: ()" {
    assert_minishell_equal_bash "()
echo \$?"
}

@test "test paren: )(" {
    assert_minishell_equal_bash ")(
echo \$?"
}

@test "test paren: ( () )" {
    assert_minishell_equal_bash "( () )
echo \$?"
}

@test "test paren: ( )( )" {
    assert_minishell_equal_bash "( )( )
echo \$?"
}

@test "test paren: ( ) ( )" {
    assert_minishell_equal_bash "( ) ( )
echo \$?"
}

# Basic command tests
@test "test paren: ls )" {
    assert_minishell_equal_bash "ls )
echo \$?"
}

@test "test paren: ( ls )( ls )" {
    assert_minishell_equal_bash "( ls )( ls )
echo \$?"
}

@test "test paren: ( ls ) ( ls )" {
    assert_minishell_equal_bash "( ls ) ( ls )
echo \$?"
}

@test "test paren: (ls)" {
    assert_minishell_equal_bash "(ls)
echo \$?
(ls)"
}

@test "test paren: (ls pwd)" {
    assert_minishell_equal_bash "(ls pwd)
echo \$?"
}

@test "test paren: (nonexistentcmd)" {
    assert_minishell_equal_bash "(nonexistentcmd)
echo \$?"
}

# Logic operators tests
@test "test paren: (ls || echo ok)" {
    assert_minishell_equal_bash "(ls || echo ok)
echo \$?"
}

@test "test paren: (ls && echo ok)" {
    assert_minishell_equal_bash "(ls && echo ok)
echo \$?"
}

@test "test paren: (true || echo ok)" {
    assert_minishell_equal_bash "(true || echo ok)
echo \$?"
}

@test "test paren: (true && echo ok)" {
    assert_minishell_equal_bash "(true && echo ok)
echo \$?"
}

@test "test paren: ((true) || echo ok)" {
    assert_minishell_equal_bash "((true) || echo ok)
echo \$?"
}

@test "test paren: ((true) && echo ok)" {
    assert_minishell_equal_bash "((true) && echo ok)
echo \$?"
}

@test "test paren: (true || (echo ok))" {
    assert_minishell_equal_bash "(true || (echo ok))
echo \$?"
}

@test "test paren: (true && (echo ok))" {
    assert_minishell_equal_bash "(true && (echo ok))
echo \$?"
}

@test "test paren: (false && echo ok)" {
    assert_minishell_equal_bash "(false && echo ok)
echo \$?"
}

@test "test paren: (false || echo ok)" {
    assert_minishell_equal_bash "(false || echo ok)
echo \$?"
}

@test "test paren: ((false) && echo ok)" {
    assert_minishell_equal_bash "((false) && echo ok)
echo \$?"
}

@test "test paren: ((false) || printf ok\n)" {
    assert_minishell_equal_bash "((false) || printf 'ok\n')
echo \$?"
}

# Pipe tests
@test "test paren: (true && echo ok) | cd .." {
    assert_minishell_equal_bash "(true && echo ok) | cd ..
echo \$?"
}

@test "test paren: (ls | wc -l) | (cat)" {
    assert_minishell_equal_bash "(ls | wc -l) | (cat)
echo \$?"
}

@test "test paren: (ls && echo ok) | cat" {
    assert_minishell_equal_bash "(ls && echo ok) | cat
echo \$?"
}

@test "test paren: (false && echo ok) | cat" {
    assert_minishell_equal_bash "(false && echo ok) | cat
echo \$?"
}

@test "test paren: (false || echo ok) | cat" {
    assert_minishell_equal_bash "(false || echo ok) | cat
echo \$?"
}

@test "test paren: ((false) || echo ok) | cat" {
    assert_minishell_equal_bash "((false) || echo ok) | cat
echo \$?"
}

@test "test paren: ls && (echo ok | cat)" {
    assert_minishell_equal_bash "ls && (echo ok | cat)
echo \$?"
}

@test "test paren: (ls | cat) | cat" {
    assert_minishell_equal_bash "(ls | cat) | cat
echo \$?"
}

# Directory operation tests
@test "test paren: (cd .. && pwd) && pwd" {
    assert_minishell_equal_bash "(cd .. && pwd) && pwd
echo \$?"
}

@test "test paren: (cd .. && (pwd)) && (cd && pwd)" {
    assert_minishell_equal_bash "(cd .. && (pwd)) && (cd && pwd)
echo \$?"
}

# Quote handling tests
@test "test paren: (echo ') with close paren in single quote str')" {
    assert_minishell_equal_bash "(echo ') with close paren in single quote str')
echo \$?"
}

@test "test paren: (echo \") with close paren in double quote str\")" {
    assert_minishell_equal_bash "(echo \") with close paren in double quote str\")
echo \$?"
}

@test "test paren: (echo '( with open paren in single quote str')" {
    assert_minishell_equal_bash "(echo '( with open paren in single quote str')
echo \$?"
}

@test "test paren: (echo \"( with open paren in double quote str\")" {
    assert_minishell_equal_bash "(echo \"( with open paren in double quote str\")
echo \$?"
}

# Complex nested tests
@test "test paren: (ls | ( ( ( (cat) ) ) ) | ( ( (cat) ) )" {
    assert_minishell_equal_bash "(ls | ( ( ( (cat) ) ) ) | ( ( (cat) ) )
echo \$?"
}

@test "test paren: (ls | ( ( ( (cat) ) ) ) ) | ( ( (cat) ) )" {
assert_minishell_equal_bash "(ls | ( ( ( (cat) ) ) ) ) | ( ( (cat) ) )
echo \$?"
}

# Redirection tests
@test "test paren: (echo hello > file) && (cat < file)" {
    assert_minishell_equal_bash "(echo hello > file) && (cat < file)
echo \$?"
}

@test "test paren: ((cat << EOF) | (grep hello)) \n hello \n world \n EOF" {
    assert_minishell_equal_bash "((cat << EOF) | (grep hello))
hello
world
EOF
echo \$?"
}

# Space handling tests
@test "test paren: (   ls    |    cat    )" {
    assert_minishell_equal_bash "(   ls    |    cat    )
echo \$?"
}

@test "test paren: (  (  (  ls  )  )  )" {
    assert_minishell_equal_bash "(  (  (  ls  )  )  )
echo \$?"
}
