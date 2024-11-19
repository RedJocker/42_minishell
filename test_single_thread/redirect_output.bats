#!/usr/bin/env bats

load parallel_helper

@test "test redirect output: > a.txt" {
    assert_minishell_equal_bash "cd $TEST_CASE_DIR
> a.txt
printf '$?\n'
ls -H"
}

@test "test redirect output: > a.txt > c.txt" {
    assert_minishell_equal_bash "cd $TEST_CASE_DIR
> a.txt > c.txt
printf '$?\n'
ls -H"
}

@test "test redirect output(simple command redirect output): echo hello > a.txt" {
    assert_minishell_equal_bash "echo hello > $TEST_CASE_DIR/a.txt
printf '$?\n'
cat $TEST_CASE_DIR/a.txt"
}

@test "test redirect output(simple command with one > redirect at end of command): ls -a \$TEST_CASE_DIR -H > a.txt" {
    assert_minishell_equal_bash "touch $TEST_CASE_DIR/a.txt
ls -a $TEST_CASE_DIR -H > $TEST_CASE_DIR/a.txt
printf '$?\n'
cat $TEST_CASE_DIR/a.txt"
}

@test "test redirect output(simple command with one > redirect between args): ls -a \$TEST_CASE_DIR > a.txt -H" {
    assert_minishell_equal_bash "touch $TEST_CASE_DIR/a.txt
ls -a $TEST_CASE_DIR > $TEST_CASE_DIR/a.txt -H
printf '$?\n'
cat $TEST_CASE_DIR/a.txt"
}

@test "test redirect output(simple command with one > redirect between invocation and arg): ls > a.txt -a \$TEST_CASE_DIR -H" {
    assert_minishell_equal_bash "touch $TEST_CASE_DIR/a.txt
ls > $TEST_CASE_DIR/a.txt -a $TEST_CASE_DIR -H
printf '$?\n'
cat $TEST_CASE_DIR/a.txt"
}

@test "test redirect output(simple command with one > redirect before invocation): > a.txt ls -a \$TEST_CASE_DIR -H" {
    assert_minishell_equal_bash "touch $TEST_CASE_DIR/a.txt
> $TEST_CASE_DIR/a.txt ls -a $TEST_CASE_DIR -H 
printf '$?\n'
cat $TEST_CASE_DIR/a.txt"
}

@test "test redirect output(simple command with two > redirects to different files): ls -a \$TEST_CASE_DIR -H > a.txt > b.txt" {
    assert_minishell_equal_bash "printf truncable > $TEST_CASE_DIR/a.txt
printf '$?\n'
cat $TEST_CASE_DIR/a.txt
ls -a $TEST_CASE_DIR -H > $TEST_CASE_DIR/a.txt > $TEST_CASE_DIR/b.txt 
printf '$?\n'
cat $TEST_CASE_DIR/a.txt
cat $TEST_CASE_DIR/b.txt"
}

@test "test redirect output(simple command with two > redirects to same file: ls -a \$TEST_CASE_DIR -H > a.txt > a.txt" {
    assert_minishell_equal_bash "ls -a $TEST_CASE_DIR -H > $TEST_CASE_DIR/a.txt > $TEST_CASE_DIR/a.txt
printf '$?\n'
cat $TEST_CASE_DIR/a.txt"
}
 
@test "test redirect output(simple command with invalid redirect syntax): ls -a \$TEST_CASE_DIR -H > > a.txt" {
    assert_minishell_equal_bash "ls -a $TEST_CASE_DIR -H > > $TEST_CASE_DIR/a.txt
printf '$?\n'"
}

@test "test redirect output(simple command with > redirection to file without permission): printf protected > a.txt \n chmod 444 a.txt \n ls > a.txt \n printf \$? \n cat a.txt" {
    assert_minishell_equal_bash "printf protected > $TEST_CASE_DIR/a.txt
printf '$?\n'
chmod 444 $TEST_CASE_DIR/a.txt
ls > $TEST_CASE_DIR/a.txt
printf '$?\n'
cat $TEST_CASE_DIR/a.txt"
}
