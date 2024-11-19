#!/usr/bin/env bats

setup() {
    load parallel_helper
}

@test "test echo: echo \"Hello World\"" {
    assert_minishell_equal_bash "echo \"Hello World\"
printf '$?\n'"
}

@test "test echo: echo my HOME = \$HOME" {
    assert_minishell_equal_bash "echo \"my HOME = \$HOME\"
printf '$?\n'"
}

@test "test echo: echo arg1" {
    assert_minishell_equal_bash "echo arg1
printf '$?\n'"
}

@test "test echo: echo arg1 arg2 arg3" {
    assert_minishell_equal_bash "echo arg1 arg2 arg3
printf '$?\n'"
}

@test "test echo: echo working > a.txt" {
    assert_minishell_equal_bash "echo working > $TEST_CASE_DIR/a.txt
printf '$?\n'
cat $TEST_CASE_DIR/a.txt
ls $TEST_CASE_DIR"
}

@test "test echo: echo > a.txt working" {
    assert_minishell_equal_bash "echo > $TEST_CASE_DIR/a.txt working
printf '$?\n'
cat $TEST_CASE_DIR/a.txt
ls $TEST_CASE_DIR"
}

@test "test echo: > a.txt echo working" {
    assert_minishell_equal_bash "> $TEST_CASE_DIR/a.txt echo working
printf '$?\n'
cat $TEST_CASE_DIR/a.txt
ls $TEST_CASE_DIR"
}

@test "test echo(builtin echo with invalid redirect syntax > >): echo what > > a.txt" {
    assert_minishell_equal_bash "echo what > > $TEST_CASE_DIR/a.txt
printf '$?\n'"
}

@test "test echo(builtin echo with > redirect to file without permission)" {
    assert_minishell_equal_bash "printf protected > $TEST_CASE_DIR/a.txt
chmod 444 $TEST_CASE_DIR/a.txt
printf override > $TEST_CASE_DIR/a.txt
printf '$?\n'
cat $TEST_CASE_DIR/a.txt"
}
