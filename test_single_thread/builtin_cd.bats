#!/usr/bin/env bats

setup() {
    load parallel_helper
}

@test "test cd: cd temp \n pwd \n cd . \n pwd \n cd .. \n pwd" {
    assert_minishell_equal_bash "mkdir -p $TEST_CASE_DIR/temp
cd $TEST_CASE_DIR/temp
printf '$?\n'
pwd
cd .
printf '$?\n'
pwd
cd ..
printf '$?\n'
pwd"
}

@test "test cd: cd temp \n echo \$PWD \n echo \$OLD_PWD \n cd . \n echo \$PWD \n echo \$OLD_PWD \n cd .. \n echo \$PWD \n echo \$OLD_PWD" {
    assert_minishell_equal_bash "mkdir -p $TEST_CASE_DIR/temp
printf '$?\n'
cd $TEST_CASE_DIR/temp
printf '$?\n'
printf '$PWD\n'
printf '$OLDPWD\n'
cd .
printf '$?\n'
printf '$PWD\n'
printf '$OLDPWD\n'
cd ..
printf '$?\n'
printf '$PWD\n'
printf '$OLDPWD\n'"
}
