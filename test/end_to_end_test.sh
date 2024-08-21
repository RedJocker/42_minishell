#!/bin/env bash -e
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    end_to_end_test.sh                                 :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/15 18:09:18 by maurodri          #+#    #+#              #
#    Updated: 2024/08/21 18:46:25 by maurodri         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


setup_file() {
    #echo "START TEST" 1>&3
    make -C .
}

setup() {
    #echo "START TEST METHOD" 1>&3
    true
}

teardown() {
    #echo "END TEST METHOD" 1>&3
    true
}

teardown_file() {
    #echo "END TEST" 1>&3
    true
}

bash_execute() {
    bash <<< "$@"
}

minishell_execute() {
    ./minishell <<< "$@"
}

minishell_leak_check() {
    valgrind --leak-check=full \
	     -s \
	     --show-reachable=yes \
	     --errors-for-leak-kinds=all \
	     --error-exitcode=1 \
	     --track-origins=yes \
	     --suppressions=mini.supp \
	     ./minishell <<< "$@"
}

assert_minishell_equal_bash() {
    run bash_execute "$@"
    local bash_status=$status
    local bash_output=$output

    #echo $bash_status 1>&3
    #echo $bash_output 1>&3

    run minishell_execute "$@"
    local mini_output=$(awk '!/^RedWillShell\$/ {print $0}' <<< "$output")
    if ! [[ $bash_output == $mini_output ]]; then
		echo -e "===> bash_output:\n<$bash_output>\n===> minishell_output:\n<$output>"
		false
    fi

    if ! [[ $bash_status == $status ]]; then
		echo -e "===> bash_status: $bash_status\nminishell_status: $status"
		false
    fi

    run minishell_leak_check "$@"
    if (( status != 0 )); then
	echo -e "VALGRIND ERROR:\n$output"
	false
    fi

}

@test "test simple command: ls" {
    assert_minishell_equal_bash ls
}

@test "test simple commands: pwd" {
    assert_minishell_equal_bash pwd
}

@test "test two simple commands in two lines of input: pwd\npwd" {
    assert_minishell_equal_bash "pwd
pwd"
}

@test "test simple command with one arg: ls -l" {
    assert_minishell_equal_bash ls -l
}


@test "test simple command with two args: ls -l -a" {
    assert_minishell_equal_bash ls -l
}
