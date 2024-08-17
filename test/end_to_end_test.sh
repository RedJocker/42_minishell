#!/bin/env bash -e
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    end_to_end_test.sh                                 :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/15 18:09:18 by maurodri          #+#    #+#              #
#    Updated: 2024/08/16 15:48:50 by maurodri         ###   ########.fr        #
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

assert_minishell_equal_bash() {
    run bash_execute "$@"
    local bash_status=$status
    local bash_output=$output

    #echo $bash_status 1>&3
    #echo $bash_output 1>&3

    run minishell_execute "$@"
    if ! [[ $bash_output == $output ]]; then
		echo -e "===> bash_output:\n<$bash_output>\n===> minishell_output:\n<$output>"
		false
    fi

    if ! [[ $bash_status == $status ]]; then
		echo -e "===> bash_status: $bash_status\nminishell_status: $status"
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
