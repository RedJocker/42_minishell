#!/usr/bin/env bats

setup() {
    load parallel_helper
}

@test "test paren: (ls)" {
    assert_minishell_equal_bash "(ls)
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
