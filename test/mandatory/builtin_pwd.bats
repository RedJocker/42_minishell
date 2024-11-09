#!/usr/bin/env bats

setup() {
    load ../setup_core
}

@test "test pwd: pwd" {
    assert_minishell_equal_bash "pwd
printf '$?\n'"
}

