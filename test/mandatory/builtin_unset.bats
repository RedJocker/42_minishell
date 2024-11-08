#!/usr/bin/env bats

setup() {
    load ../setup_core
}

@test "test unset: unset PATH" {
    assert_minishell_equal_bash "unset PATH
/bin/printf '$?\n'
ls -H
/bin/printf '$?\n'"
}

@test "test unset: unset aaa" {
    assert_minishell_equal_bash "unset aaa
printf '$?\n'"
}

@test "test unset: unset PATH HOME" {
    assert_minishell_equal_bash "unset PATH HOME
/bin/printf '$?\n'
ls -H
/bin/printf '$?\n'
/bin/printf '$HOME\n'
/bin/printf '$?\n'"
}

@test "test unset: unset PATH aaa HOME" {
    assert_minishell_equal_bash "unset PATH aaa HOME
/bin/printf '$?\n'
ls -H
/bin/printf '$?\n'
/bin/printf '$HOME\n'
/bin/printf '$?\n'"
}

@test "test unset: unset aaa PATH HOME" {
    assert_minishell_equal_bash "unset aaa PATH HOME
/bin/printf '$?\n'
ls -H
/bin/printf '$?\n'
/bin/printf '$HOME\n'
/bin/printf '$?\n'"
}
