#!/bin/env bash -e
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    end_to_end_test.sh                                 :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/15 18:09:18 by maurodri          #+#    #+#              #
#    Updated: 2024/09/24 23:32:36 by maurodri         ###   ########.fr        #
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
    LANGUAGE="en_US.UTF-8" PS1='RedWillShell$ ' bash --norc -i <<< "$@"
    delete_temp_folder
}

minishell_execute() {
    create_temp_folder
    ./minishell <<< "$@"
    delete_temp_folder
}

minishell_leak_check() {
    create_temp_folder
    valgrind --leak-check=full \
	     -s \
	     --show-reachable=yes \
	     --errors-for-leak-kinds=all \
	     --error-exitcode=1 \
	     --track-origins=yes \
	     --track-fds=yes \
	     --suppressions=mini.supp \
	     ./minishell <<< "$@"
    delete_temp_folder
}

assert_minishell_equal_bash() {
    run bash_execute "$@"
    local bash_status=$status
    local bash_output=$output

    #echo $bash_status 1>&3
    #echo "$bash_output" 1>&3
    
    #local bash_out_norm=$(awk 'NR > 2 && !/^RedWillShell\$/ { print $0}' <<< "$output")
    
    run minishell_execute "$@"
    #local mini_output=$(awk '!/^RedWillShell\$/ {print $0}' <<< "$output")

    #echo -e "===> bash_output:\n<$bash_output>\n===> minishell_output:\n<$output>" 1>&3
    if ! [[ $bash_output == $output ]]; then
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

# TEST BEGIN

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
    assert_minishell_equal_bash ls -l -a
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

@test "test simple command with two > redirects to different files: ls -a \$temp_dir -H > \$file1 > \$file2" {
    file1="$temp_dir/a.txt"
    file2="$temp_dir/b.txt"
    assert_minishell_equal_bash "
printf truncable > $file1
cat $file1
ls -a $temp_dir -H > $file1 > $file2 
cat $file1
cat $file2
"
}

@test "test simple command with two > redirects to same file: ls -a \$temp_dir -H > \$file1 > \$file1" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "ls -a $temp_dir -H > $file1 > $file1 
cat $file1
"
}
 
@test "test simple command with invalid redirect syntax" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "ls -a $temp_dir -H > > $file1
printf \$?"
}

@test "test simple command with > redirection to file without permission " {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "printf protected > $file1
chmod 444 $file1
ls > $file1
printf \$?
cat $file1
"
}

@test "test builtin echo with one arg" {
    assert_minishell_equal_bash "echo testing
"
}

@test "test builtin echo with two args" {
    assert_minishell_equal_bash "echo testing two
"
}

@test "test builtin echo with > redirection end" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "ls $temp_dir 
echo working > $file1
cat $file
ls $temp_dir
"
}

@test "test builtin echo with > redirection middle" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "ls $temp_dir 
echo > $file1 working
cat $file
ls $temp_dir
"
}

@test "test builtin echo with > redirection start" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "ls $temp_dir 
> $file1 echo working
cat $file
ls $temp_dir
"
}

@test "test builtin echo with invalid redirect syntax > >" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "echo what > > $file1
echo \$?"
}

@test "test builtin echo with > redirection to file without permission " {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "printf protected > $file1
chmod 444 $file1
echo override > $file1
echo \$?
cat $file1
"
}

@test "test simple command with one >> redirect at end of command: ls -a \$temp_dir -H >> \$file" {
    file="$temp_dir/a.txt"
    assert_minishell_equal_bash "ls -a $temp_dir -H >> $file
cat $file
uname >> $file
cat $file
"
}

@test "test simple command with one >> redirect at middle of command: ls -a  >> \$file \$temp_dir -H" {
    file="$temp_dir/a.txt"
    assert_minishell_equal_bash "ls -a >> $file $temp_dir -H
cat $file
uname >> $file
cat $file
"
}

@test "test simple command with one >> redirect at start of command: >> \$file ls -a \$temp_dir -H" {
    file="$temp_dir/a.txt"
    assert_minishell_equal_bash ">> $file ls -a $temp_dir -H
cat $file
uname >> $file
cat $file
"
}

@test "test simple command with invalid redirect syntax > >>" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "ls -a $temp_dir -H > >> '$file1'
printf \$?"
}

@test "test simple command with invalid redirect syntax >> >" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "ls -a $temp_dir -H >> > '$file1'
printf \$?"
}

@test "test simple command with invalid redirect syntax >> >>" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "ls -a $temp_dir -H >> >> '$file1'
printf \$?"
}

@test "test simple command with >> redirection to file without permission " {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "printf protected >> $file1
chmod 444 $file1
ls >> $file1
printf \$?
cat $file1
"
}

@test "test simple command with two >> redirects to different files: ls -a \$temp_dir -H >> \$file1 >> \$file2" {
    file1="$temp_dir/a.txt"
    file2="$temp_dir/b.txt"
    assert_minishell_equal_bash "
printf non-truncable > $file1
printf non-truncable > $file2
cat $file1
cat $file2
ls -a $temp_dir -H >> $file1 >> $file2 
cat $file1
cat $file2
"
}

@test "test simple command with two >> redirects to same file: ls -a \$temp_dir -H >> \$file1 >> \$file1" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "
printf non-truncable > $file1
cat $file1
ls -a $temp_dir -H >> $file1 >> $file1 
cat $file1
"
}

@test "test simple command with >> and > redirects to same file: ls -a \$temp_dir -H >> \$file1 > \$file1" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "
printf truncable > $file1
cat $file1
ls -a $temp_dir -H >> $file1 > $file1 
cat $file1
"
}

@test "test simple command with > and >> redirects to same file: ls -a \$temp_dir -H > \$file1 >> \$file1" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "
printf truncable > $file1
cat $file1
ls -a $temp_dir -H > $file1 >> $file1 
cat $file1
"
}

@test "test simple command with < redirection at end: cat < \$file1 " {
    file1="$temp_dir/input.txt"
    assert_minishell_equal_bash "printf input > $file1
cat < $file1
"
}

@test "test simple command with invalid redirect syntax > <" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "ls -a $temp_dir -H > < $file1
printf \$?"
}

@test "test simple command with invalid redirect syntax < >" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "ls -a $temp_dir -H < > $file1
printf \$?"
}

@test "test simple command with invalid redirect syntax >> <" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "ls -a $temp_dir -H >> < '$file1'
printf \$?"
}

@test "test simple command with invalid redirect syntax < >>" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "ls -a $temp_dir -H < >> '$file1'
printf \$?"
}

@test "test simple command with invalid redirect syntax < <" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "ls -a $temp_dir -H < < $file1
printf \$?"
}

@test "test simple command with < redirection from file without permission " {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "printf protected >> $file1
chmod 000 $file1
< $file1 cat
printf \$?
"
}

@test "test simple command with < redirection from file that does not exist" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "printf excludable > $file1
rm $file1
< $file1 cat
printf \$?
"
}

@test "test simple command with < and > redirects to same file: ls -a \$temp_dir -H < \$file1 > \$file1" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "
printf clonable > $file1
cat $file1
ls -a $temp_dir -H < $file1 > $file1 
cat $file1
"
}

@test "test simple command with < and >> redirects to same file: ls -a \$temp_dir -H < \$file1 >> \$file1" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "
printf duplicate > $file1
cat $file1
ls -a $temp_dir -H < $file1 >> $file1 
cat $file1
"
}

@test "test simple command with single quote on args: printf 'hello'" {
    assert_minishell_equal_bash "
printf 'hello'
"
}

@test "test simple command with single quote on invocation: 'printf' hello" {
    assert_minishell_equal_bash "
'printf' hello
"
}

@test "test simple command with single quote on arg with space: printf 'hello there'" {
    assert_minishell_equal_bash "
printf 'hello there' 
"
}

@test "test simple command with single quote on middle of arg: printf he'll'o" {
    assert_minishell_equal_bash "
printf he'll'o 
"
}

@test "test simple command with single quote on middle of invocation: p'rin'tf hello" {
    assert_minishell_equal_bash "
p'rin'tf hello 
"
}

@test "test simple command with invalid redirect syntax: ls >" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "ls >
printf \$?"
}


@test "test simple command with invalid redirect syntax: ls >>" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "ls >>
printf \$?"
}


@test "test simple command with invalid redirect syntax: ls <" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "ls <
printf \$?"
}

@test "test builtin echo with one >> redirect at end of command: echo next >> \$file" {
    file="$temp_dir/a.txt"
    assert_minishell_equal_bash "echo previous > $file
cat $file
echo next >> $file
cat $file
"
}

@test "test builtin echo with one >> redirect at middle of command: echo hello  >> \$file there" {
    file="$temp_dir/a.txt"
    assert_minishell_equal_bash "echo previous > $file
cat $file
echo hello >> $file there
cat $file
"
}

@test "test builtin echo with one >> redirect at start of command: >> \$file echo next" {
    file="$temp_dir/a.txt"
    assert_minishell_equal_bash "echo previous > $file
cat $file
>> $file echo next
cat $file
"
}

@test "test builtin echo invalid redirect syntax > >>" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "echo hello > >> $file1
echo \$?"
}

@test "test builtin echo with invalid redirect syntax >> >" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "echo hello >> > $file1
echo \$?"
}

@test "test builtin echo with invalid redirect syntax >> >>" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "ls -a $temp_dir -H >> >> $file1
echo \$?"
}

@test "test builtin echo with >> redirection to file without permission " {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "printf protected >> $file1
chmod 444 $file1
echo 'should not write' >> $file1
echo \$?
cat $file1
"
}

@test "test builtin echo with two >> redirects to different files: echo append only file2 >> \$file1 >> \$file2 " {
    file1="$temp_dir/a.txt"
    file2="$temp_dir/b.txt"
    assert_minishell_equal_bash "
echo should not append > $file1
echo should append > $file2
cat $file1
cat $file2
echo append only file2 >> $file1 >> $file2 
cat $file1
cat $file2
"
}

@test "test builtin echo with two >> redirects to same file: echo append once >> \$file1 >> \$file1 " {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "
echo 'content ' > $file1
cat $file1
echo append once >> $file1 >> $file1 
cat $file1
"
}

@test "test builtin echo with >> and > redirects to same file: echo write over >> \$file1 > \$file1 " {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "
echo truncable > $file1
cat -e $file1
echo write over >> $file1 > $file1 
cat -e $file1
"
}

@test "test builtin echo with > and >> redirects to same file: echo overwrite > \$file1 >> \$file1" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "
echo truncable > $file1
cat $file1
echo overwrite > $file1 >> $file1
cat $file1
"
}

@test "test builtin echo with < redirection at end: echo ignore input < $file1" {
    file1="$temp_dir/input.txt"
    assert_minishell_equal_bash "printf input > $file1
echo ignore input < $file1 
"
}

@test "test builtin echo with invalid redirect syntax > <" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "echo fail > < $file1
echo \$?"
}

@test "test builtin echo with invalid redirect syntax < >" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "echo fail < > $file1
echo \$?"
}

@test "test builtin echo with invalid redirect syntax >> <" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "echo fail >> < $file1
echo \$?"
}

@test "test builtin echo with invalid redirect syntax < >>" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "echo fail < >> $file1
echo \$?"
}

@test "test builtin echo with invalid redirect syntax < <" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "echo fail < < $file1
echo \$?"
}

@test "test builtin echo with < redirection from file without permission " {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "echo protected >> $file1
chmod 000 $file1
< $file1 echo should deny
echo \$?
"
}

@test "test builtin echo with < redirection from file that does not exist" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "echo excludable > $file1
rm $file1
< $file1 echo does not exist
echo \$?
"
}

@test "test builtin echo with < and > redirects to same file: echo ignore input and write < \$file1 > \$file1 " {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "
echo clonable > $file1
cat $file1
echo ignore input and write < $file1 > $file1 
cat $file1
"
}

@test "test builtin echo with < and >> redirects to same file: ls -a \$temp_dir -H < \$file1 >> \$file1" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "
echo duplicate > $file1
cat $file1
echo fail < $file1 >> $file1 
cat $file1
"
}

@test "test builtin echo with single quote on args: echo 'hello'" {
    assert_minishell_equal_bash "
echo 'hello'
"
}

@test "test builtin echo with single quote on invocation: 'echo' hello" {
    assert_minishell_equal_bash "
'echo' hello
"
}

@test "test builtin echo with single quote on arg with space: echo 'hello there'" {
    assert_minishell_equal_bash "
echo 'hello there' 
"
}

@test "test builtin echo with single quote on middle of arg: echo he'll'o" {
    assert_minishell_equal_bash "
echo he'll'o 
"
}

@test "test builtin echo with single quote on middle of invocation: e'ch'o hello" {
    assert_minishell_equal_bash "
e'ch'o hello 
"
}

@test "test builtin echo with invalid redirect syntax: echo hello >" {
    assert_minishell_equal_bash "echo hello >
echo \$?"
}


@test "test builtin echo with invalid redirect syntax: echo hello >>" {
    assert_minishell_equal_bash "ls >>
echo \$?"
}

@test "test builtin echo with invalid redirect syntax: echo hello <" {
    assert_minishell_equal_bash "ls <
echo \$?"
}

@test "test pipe: ls | wc" {
    assert_minishell_equal_bash "ls | wc"
}

@test "test pipe: ls -a | wc" {
    assert_minishell_equal_bash "ls -a | wc"
}

@test "test pipe: ls | wc -c" {
    assert_minishell_equal_bash "ls | wc -c"
}

@test "test pipe: ls > \$file1 | wc" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "ls > $file1 | wc"
}

@test "test pipe: ls < \$file1 | wc" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash " echo ignored > $file1
ls < $file1 | wc"
}

@test "test pipe: ls | wc < \$file1" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash " echo 123 > $file1
ls | cat < $file1"
}

@test "test pipe: ls | wc | cat" {
    assert_minishell_equal_bash "ls | wc | cat"
}

@test "test pipe: ls -l | wc | cat" {
    assert_minishell_equal_bash "ls -l | wc | cat"
}

@test "test pipe: ls | wc -c | cat" {
    assert_minishell_equal_bash "ls | wc -c | cat"
}

@test "test pipe: ls | wc | cat -e" {
    assert_minishell_equal_bash "ls | wc | cat -e"
}
