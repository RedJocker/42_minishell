#!/usr/bin/env bats

load ../setup_core

# Basic syntax tests
@test "parenthesis: )" {
    assert_minishell_equal_bash ")
echo Status code: \$?"
}

@test "parenthesis: ()" {
    assert_minishell_equal_bash "()
echo Status code: \$?"
}

@test "parenthesis: )(" {
    assert_minishell_equal_bash ")(
echo Status code: \$?"
}

@test "parenthesis: ( () )" {
    assert_minishell_equal_bash "( () )
echo Status code: \$?"
}

@test "parenthesis: ( )( )" {
    assert_minishell_equal_bash "( )( )
echo Status code: \$?"
}

@test "parenthesis: ( ) ( )" {
    assert_minishell_equal_bash "( ) ( )
echo Status code: \$?"
}

# Basic command tests
@test "parenthesis: ls )" {
    assert_minishell_equal_bash "ls )
echo Status code: \$?"
}

@test "parenthesis: ( ls )( ls )" {
    assert_minishell_equal_bash "( ls )( ls )
echo Status code: \$?"
}

@test "parenthesis: ( ls ) ( ls )" {
    assert_minishell_equal_bash "( ls ) ( ls )
echo Status code: \$?"
}

@test "parenthesis: (ls)" {
    assert_minishell_equal_bash "(ls)
echo Status code: \$?"
}

@test "parenthesis: (ls pwd)" {
    assert_minishell_equal_bash "(ls pwd)
echo Status code: \$?"
}

@test "parenthesis: (nonexistentcmd)" {
    assert_minishell_equal_bash "(nonexistentcmd)
echo Status code: \$?"
}

# Logic operators tests
@test "parenthesis: (ls || echo ok)" {
    assert_minishell_equal_bash "(ls || echo ok)
echo Status code: \$?"
}

@test "parenthesis: (ls && echo ok)" {
    assert_minishell_equal_bash "(ls && echo ok)
echo Status code: \$?"
}

@test "parenthesis: (true || echo ok)" {
    assert_minishell_equal_bash "(true || echo ok)
echo Status code: \$?"
}

@test "parenthesis: (true && echo ok)" {
    assert_minishell_equal_bash "(true && echo ok)
echo Status code: \$?"
}

@test "parenthesis: ((true) || echo ok)" {
    assert_minishell_equal_bash "((true) || echo ok)
echo Status code: \$?"
}

@test "parenthesis: ((true) && echo ok)" {
    assert_minishell_equal_bash "((true) && echo ok)
echo Status code: \$?"
}

@test "parenthesis: (true || (echo ok))" {
    assert_minishell_equal_bash "(true || (echo ok))
echo Status code: \$?"
}

@test "parenthesis: (true && (echo ok))" {
    assert_minishell_equal_bash "(true && (echo ok))
echo Status code: \$?"
}

@test "parenthesis: (false && echo ok)" {
    assert_minishell_equal_bash "(false && echo ok)
echo Status code: \$?"
}

@test "parenthesis: (false || echo ok)" {
    assert_minishell_equal_bash "(false || echo ok)
echo Status code: \$?"
}

@test "parenthesis: ((false) && echo ok)" {
    assert_minishell_equal_bash "((false) && echo ok)
echo Status code: \$?"
}

@test "parenthesis: ((false) || printf ok\n)" {
    assert_minishell_equal_bash "((false) || printf 'ok\n')
echo Status code: \$?"
}

# Pipe tests
@test "parenthesis: (true && echo ok) | cd .." {
    assert_minishell_equal_bash "(true && echo ok) | cd ..
echo Status code: \$?"
}

@test "parenthesis: (ls | wc -l) | (cat)" {
    assert_minishell_equal_bash "(ls | wc -l) | (cat)
echo Status code: \$?"
}

@test "parenthesis: (ls && echo ok) | cat" {
    assert_minishell_equal_bash "(ls && echo ok) | cat
echo Status code: \$?"
}

@test "parenthesis: (false && echo ok) | cat" {
    assert_minishell_equal_bash "(false && echo ok) | cat
echo Status code: \$?"
}

@test "parenthesis: (false || echo ok) | cat" {
    assert_minishell_equal_bash "(false || echo ok) | cat
echo Status code: \$?"
}

@test "parenthesis: ((false) || echo ok) | cat" {
    assert_minishell_equal_bash "((false) || echo ok) | cat
echo Status code: \$?"
}

@test "parenthesis: ls && (echo ok | cat)" {
    assert_minishell_equal_bash "ls && (echo ok | cat)
echo Status code: \$?"
}

@test "parenthesis: (ls | cat) | cat" {
    assert_minishell_equal_bash "(ls | cat) | cat
echo Status code: \$?"
}

# Directory operation tests
@test "parenthesis: (cd .. && pwd) && pwd" {
    assert_minishell_equal_bash "(cd .. && pwd) && pwd
echo Status code: \$?"
}

@test "parenthesis: (cd .. && (pwd)) && (cd && pwd)" {
    assert_minishell_equal_bash "(cd .. && (pwd)) && (cd && pwd)
echo Status code: \$?"
}

# Quote handling tests
@test "parenthesis: (echo ') with close parenthesis in single quote str')" {
    assert_minishell_equal_bash "(echo ') with close parenthesis in single quote str')
echo Status code: \$?"
}

@test "parenthesis: (echo \") with close parenthesis in double quote str\")" {
    assert_minishell_equal_bash "(echo \") with close parenthesis in double quote str\")
echo Status code: \$?"
}

@test "parenthesis: (echo '( with open parenthesis in single quote str')" {
    assert_minishell_equal_bash "(echo '( with open parenthesis in single quote str')
echo Status code: \$?"
}

@test "parenthesis: (echo \"( with open parenthesis in double quote str\")" {
    assert_minishell_equal_bash "(echo \"( with open parenthesis in double quote str\")
echo Status code: \$?"
}

# Complex nested tests
@test "parenthesis: (ls | ( ( ( (cat) ) ) ) ) | ( ( (cat) ) )" {
	assert_minishell_equal_bash "(ls | ( ( ( (cat) ) ) ) ) | ( ( (cat) ) )
echo Status code: \$?"
}

# Redirection tests
@test "parenthesis: (echo hello > file) && (cat < file)" {
    assert_minishell_equal_bash "(echo hello > file) && (cat < file)
echo Status code: \$?"
}

@test "parenthesis: ((cat << EOF) | (grep hello)) \n hello \n world \n EOF" {
    assert_minishell_equal_bash "((cat << EOF) | (grep hello))
hello
world
EOF
echo Status code: \$?"
}

# Space handling tests
@test "parenthesis: (   ls    |    cat    )" {
    assert_minishell_equal_bash "(   ls    |    cat    )
echo Status code: \$?"
}

@test "parenthesis: (  (  (  ls  )  )  )" {
    assert_minishell_equal_bash "(  (  (  ls  )  )  )
echo Status code: \$?"
}
