#!/usr/bin/env bats

load ../setup_core

@test "test pwd: pwd" {
    assert_minishell_equal_bash "pwd
printf '$?\n'"
}

