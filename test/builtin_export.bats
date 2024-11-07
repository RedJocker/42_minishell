#!/usr/bin/env bats

setup() {
    load parallel_helper
}

@test "test export: export" {
    assert_minishell_equal_bash "export | grep -v -i 'bats' | grep -v '}' | grep -v '_=' | grep -v '$'
printf '$?\n'"
}

@test "test export: export aaa" {
    assert_minishell_equal_bash "export aaa
printf '$?\n'
export | grep -v -i 'bats' | grep -v '}' | grep -v '_=' | grep -v '$'
printf '$?\n'"
}

@test "test export: export aaa=test" {
    assert_minishell_equal_bash "export aaa=test
printf '$?\n'
export | grep -v -i 'bats' | grep -v '}' | grep -v '_=' | grep -v '$'
printf '$?\n'"
}

@test "test export: export aaa=\"test test\"" {
    assert_minishell_equal_bash "export aaa=\"test test\"
printf '$?\n'
export | grep -v -i 'bats' | grep -v '}' | grep -v '_=' | grep -v '$'
printf '$?\n'"
}

@test "test export: export aaa \n export aaa=test" {
    assert_minishell_equal_bash "export aaa
printf '$?\n'
export | grep -v -i 'bats' | grep -v '}' | grep -v '_=' | grep -v '$'
printf '$?\n'
export aaa=test
printf '$?\n'
export | grep -v -i 'bats' | grep -v '}' | grep -v '_=' | grep -v '$'
printf '$?\n'"
}

@test "test export: export -aaa" {
    assert_minishell_equal_bash "export -aaa
printf '$?\n'
export | grep -v -i 'bats' | grep -v '}' | grep -v '_=' | grep -v '$'
printf '$?\n'"
}

@test "test export: export 1aaa" {
    assert_minishell_equal_bash "export 1aaa
printf '$?\n'
export | grep -v -i 'bats' | grep -v '}' | grep -v '_=' | grep -v '$'
printf '$?\n'"
}

@test "test export: 1aaa=test" {
    assert_minishell_equal_bash "export 1aaa=test
printf '$?\n'
export | grep -v -i 'bats' | grep -v '}' | grep -v '_=' | grep -v '$'
printf '$?\n'"
}
