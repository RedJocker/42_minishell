#!/bin/env bash -e
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    end_to_end_test.sh                                 :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/15 18:09:18 by maurodri          #+#    #+#              #
#    Updated: 2024/08/30 23:13:47 by maurodri         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

setup_file() {
    #echo "START TEST" 1>&3
    true
}

setup() {
    #echo "START TEST METHOD" 1>&3
    temp_dir="./test/temp$$"
}

teardown() {
    #echo "END TEST METHOD" 1>&3
    true
}

teardown_file() {
    #echo "END TEST" 1>&3
    true
}

create_temp_folder() {
    mkdir $temp_dir
}

delete_temp_folder() {
    rm -rf $temp_dir
}

bash_execute() {
    create_temp_folder
    bash <<< "$@"
    delete_temp_folder
}

minishell_execute() {
    create_temp_folder
    ./minishell <<< "$@"
    delete_temp_folder
}

minishell_leak_check() {
    valgrind --leak-check=full \
	     -s \
	     --show-reachable=yes \
	     --errors-for-leak-kinds=all \
	     --error-exitcode=1 \
	     --track-origins=yes \
	     --track-fds=yes \
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

    #echo "$output" 1>&3

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

@test "test empty" {
    assert_minishell_equal_bash ""
}

@test "test empty line" {
    assert_minishell_equal_bash "
"
}

@test "test blank line" {
    assert_minishell_equal_bash "	    
"
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

@test "test simple command absolute path: /usr/bin/ls" {
    assert_minishell_equal_bash /usr/bin/ls
}

@test "test simple command with one arg: ls -l" {
    assert_minishell_equal_bash ls -l
}

@test "test simple command with two args: ls -l -a" {
    assert_minishell_equal_bash ls -l
}

@test "test simple command with one > redirect at end of command: ls -a \$temp_dir -H > \$file" {
    file="$temp_dir/a.txt"
    assert_minishell_equal_bash "ls -a $temp_dir -H > $file
cat $file"
}

@test "test simple command with one > redirect between args: ls -a \$temp_dir > \$file -H" {
    file="$temp_dir/a.txt"
    assert_minishell_equal_bash "ls -a $temp_dir > $file -H 
cat $file"
}

@test "test simple command with one > redirect between invocation and arg: ls > \$file -a \$temp_dir -H" {
    file="$temp_dir/a.txt"
    assert_minishell_equal_bash "ls > $file -a $temp_dir -H 
cat $file"
}

@test "test simple command with one > redirect before invocation: > \$file ls -a \$temp_dir -H" {
    file="$temp_dir/a.txt"
    assert_minishell_equal_bash "> $file ls -a $temp_dir -H 
cat $file"
}

@test "test simple command with two > redirects to different files: ls -a $temp_dir -H > $file1 > $file2" {
    file1="$temp_dir/a.txt"
    file2="$temp_dir/b.txt"
    assert_minishell_equal_bash "ls -a $temp_dir -H > $file1 > $file2 
echo ===$file1===
cat $file1
echo "===$file2==="
cat $file2
"
}

@test "test simple command with two > redirects to same file: ls -a $temp_dir -H > $file1 > $file1" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "ls -a $temp_dir -H > $file1 > $file1 
echo ===$file1===
cat $file1
"
}
