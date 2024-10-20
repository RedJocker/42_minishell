#!/bin/env bash -e
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    end_to_end_test.sh                                 :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maurodri <maurodri@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/15 18:09:18 by maurodri          #+#    #+#              #
#    Updated: 2024/10/19 17:33:21 by maurodri         ###   ########.fr        #
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
    delete_temp_folder
}

create_temp_folder() {
    mkdir $temp_dir
}

delete_temp_folder() {
    rm -rf $temp_dir
}

bash_execute() {
    create_temp_folder
    VARIABLE_FROM_OUTSIDE_MORE_SPACES="abc    def" VARIABLE_FROM_OUTSIDE="abc def" LANGUAGE="en" PS1='RedWillShell$ ' bash --norc -i <<< "$@"
}

minishell_execute() {
    create_temp_folder
    VARIABLE_FROM_OUTSIDE_MORE_SPACES="abc    def" VARIABLE_FROM_OUTSIDE="abc def" LANGUAGE="en" PS1='RedWillShell$ ' ./minishell <<< "$@"
}

minishell_leak_check() {
    
    valgrind --leak-check=full \
	     -s \
	     --show-reachable=yes \
	     --errors-for-leak-kinds=all \
	     --error-exitcode=33 \
	     --track-origins=yes \
	     --track-fds=yes \
	     --suppressions=mini.supp \
	     ./minishell <<< "$@"
}

assert_minishell_equal_bash() {
    run bash_execute "$@"
    delete_temp_folder
    
    local bash_status=$status
    local bash_output=$output

    #echo $bash_status 1>&3
    #echo "$bash_output" 1>&3
    
    #local bash_out_norm=$(awk 'NR > 2 && /here-document at line/ { gsub(/at line [0-9]+ /, "", $0); print $0"ddd"} !/here-document at line/ { print $0 "abc"}' <<< "$output")

    run minishell_execute "$@"
    delete_temp_folder

    #local mini_output=$(awk '!/^RedWillShell\$/ {print $0}' <<< "$output")

    #echo -e "===> bash_output:\n<$bash_output>\n===> minishell_output:\n<$output>" 1>&3 
    if ! [[ $bash_output == $output ]]; then
	        
	# echo "$bash_output" > "./test/bash_$BATS_TEST_NAME.txt"
	# echo "$output" > "./test/mini_$BATS_TEST_NAME.txt"
	echo -e "===> bash_output:\n<$bash_output>\n===> minishell_output:\n<$output>"
	false
    fi

    #echo "$output" 1>&3

    if ! [[ $bash_status == $status ]]; then
		echo -e "===> bash_status: $bash_status\nminishell_status: $status"
		false
    fi

    create_temp_folder
    run minishell_leak_check "$@"
    delete_temp_folder
    
    if (( status == 33 )); then
	echo -e "VALGRIND ERROR:\n$output"
	false
    fi
}


assert_minishell_equal_bash_heredoc() {
    run bash_execute "$@"
    delete_temp_folder
    
    local bash_status=$status
    local bash_output=$output

    #echo $bash_status 1>&3
    #echo "$bash_output" 1>&3

    local bash_out_norm=$(awk 'NR > 2 && /here-document at line/ { gsub(/at line [0-9]+ /, "", $0); print $0} !/here-document/ { print $0}' <<< "$output")

    
    run minishell_execute "$@"
    delete_temp_folder
    
    #local mini_output=$(awk '!/^RedWillShell\$/ {print $0}' <<< "$output")

    #echo -e "===> bash_out_norm:\n<$bash_out_norm>\n===> minishell_output:\n<$output>" 1>&3
    if ! [[ $bash_out_norm == $output ]]; then
		echo -e "===> bash_out_norm:\n<$bash_out_norm>\n===> minishell_output:\n<$output>"
		false
    fi

    #echo "$output" 1>&3

    if ! [[ $bash_status == $status ]]; then
		echo -e "===> bash_status: $bash_status\nminishell_status: $status"
		false
    fi
    
    create_temp_folder
    run minishell_leak_check "$@"
    delete_temp_folder
    
    if (( status == 33 )); then
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

@test "test simple commands: uname" {
    assert_minishell_equal_bash uname
}

@test "test two simple commands in two lines of input: uname\nuname" {
    assert_minishell_equal_bash "uname
uname"
}

@test "test simple command absolute path: /usr/bin/ls" {
    assert_minishell_equal_bash /usr/bin/ls
}

@test "test simple command with one arg: ls -H" {
    assert_minishell_equal_bash ls -H
}

@test "test simple command with two args: ls -H -a" {
    assert_minishell_equal_bash ls -H -a
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

@test "test builtin echo with only option -n: echo -n" {
    assert_minishell_equal_bash "echo -n
echo \$?"
}

@test "test builtin echo with option -n: echo -n hello" {
    assert_minishell_equal_bash "echo -n hello
echo \$?"
}

@test "test builtin echo with option -n: echo -nnnn hello" {
    assert_minishell_equal_bash "echo -nnnn hello
echo \$?"
}

@test "test builtin echo with option -n: echo -nnnna hello" {
    assert_minishell_equal_bash "echo -nnnna hello
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
    assert_minishell_equal_bash "ls > $file1 | wc
cat $file1"
}

@test "test pipe: ls | wc > \$file1" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "
ls | wc > $file1
cat $file1
"
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

@test "test pipe: ls > \$file1 | wc > \$file2 | cat -e > \$file3" {
    file1="$temp_dir/a.txt"
    file2="$temp_dir/b.txt"
    file3="$temp_dir/c.txt"
    assert_minishell_equal_bash "ls > $file1 | wc > $file2 | cat -e > $file3
cat $file1
cat $file2
cat $file3
"
}

@test "test pipe: ls > \$file1| wc > $file2" {
    file1="$temp_dir/a.txt"
    file2="$temp_dir/b.txt"
    assert_minishell_equal_bash "ls > $file1 | wc > $file2
    echo \$?
    cat $file1 $file2 
"
}

@test "test pipe: > \$file1 ls | > $file2 wc" {
    file1="$temp_dir/a.txt"
    file2="$temp_dir/b.txt"
    assert_minishell_equal_bash "> $file1 ls | > $file2 wc
    echo \$?
    cat $file1 $file2 
"
}

@test "test pipe: ls > \$file1 -a | wc > $file2 -c" {
    file1="$temp_dir/a.txt"
    file2="$temp_dir/b.txt"
    assert_minishell_equal_bash "ls > $file1 -a | wc > $file2 -c
    echo \$?
    cat $file1 $file2 
"
}

@test "test pipe: ls -a > \$file1 -H | wc -w > $file2 -c" {
    file1="$temp_dir/a.txt"
    file2="$temp_dir/b.txt"
    assert_minishell_equal_bash "ls -a > $file1 -H | wc -w > $file2 -c
    echo \$?
    cat $file1 $file2
"
}

@test "test pipe: < \$file1 cat | < \$file2 cat  " {
    file1="$temp_dir/a.txt"
    file2="$temp_dir/b.txt"
    assert_minishell_equal_bash "ls -a > $file1 -H | wc -w > $file2 -c
    echo \$?
    < $file1 cat | < $file2 cat  
"
}

@test "test pipe: <\$file1 cat<$file1|<\$file2 cat<$file2  " {
    file1="$temp_dir/a.txt"
    file2="$temp_dir/b.txt"
    assert_minishell_equal_bash "ls -a>$file1 -H | wc -w>$file2 -c
    echo \$?
    <$file1 cat|<$file2 cat  
"
}

@test "test pipe heredoc: cat << eof | wc" {
    assert_minishell_equal_bash "cat << eof | wc
some heredoc
text
eof
"
}

@test "test pipe heredoc: ls | wc << eof" {
   
    assert_minishell_equal_bash "ls | wc -l << eof
some heredoc
text
eof
"
}

@test "test pipe heredoc: ls << eof | wc -l << eof" {
    assert_minishell_equal_bash "ls << eof | wc -l << eof
some heredoc
text
eof
another
different
text
with more
lines
eof
"
}

@test "test pipe heredoc: << eof ls | << eof wc -l" {
    assert_minishell_equal_bash "<< eof ls | << eof wc -l
some heredoc
text
eof
another
different
text
with more
lines
eof
"
}

@test "test invalid command: echo_heredoc" {
    assert_minishell_equal_bash "echo_heredoc
"
}

@test "test invalid command: eof" {
    assert_minishell_equal_bash "eof
echo \$?"
}

@test "test invalid_command does not exist" {
    file1="$temp_dir/does_not_exist"
    assert_minishell_equal_bash "
rm -rf $file1 2> /dev/null
$file1
echo \$?"
}

@test "test invalid_command is not executable" {
    file1="$temp_dir/text"
    assert_minishell_equal_bash "
echo 'some text' > $file1 
$file1
echo \$?"
}

@test "test invalid_command is directory with execute permission" {
    assert_minishell_equal_bash "
chmod 777 $temp_dir
$temp_dir
echo \$?"
}

@test "test invalid_command is directory without execute permission" {
    assert_minishell_equal_bash "
chmod 000 $temp_dir
$temp_dir
echo \$?"
}

@test "test environment variables" {
    assert_minishell_equal_bash "echo LANG=\$LANG LC_ALL=\$LC_ALL LANGUAGE=\$LANGUAGE"
}

@test "test environment variables that exist from outside" {
    assert_minishell_equal_bash "echo xxx\$VARIABLE_FROM_OUTSIDE\"\$VARIABLE_FROM_OUTSIDE\"xxx"
}

@test "test environment variables that exist from outside more spaces" {
    assert_minishell_equal_bash "echo xxx\$VARIABLE_FROM_OUTSIDE_MORE_SPACES\"\$VARIABLE_FROM_OUTSIDE_MORE_SPACES\"xxx"
}

@test "test simple expand invalid enviroment variable: echo \$INVALID_VARIABLE" {
    assert_minishell_equal_bash echo "\$INVALID_VARIABLE"
}

@test "test pipe and echo: echo before pipe | wc" {
    assert_minishell_equal_bash "echo before pipe | cat"
}

@test "test pipe and echo: ls | echo after pipe" {
    assert_minishell_equal_bash "ls | echo after pipe"
}

@test "test pipe and echo: echo -n before without endline | wc -c" {
    assert_minishell_equal_bash "echo -n before without endline | wc -c"
}

@test "test pipe and echo:  ls | echo -n after without endline" {
    assert_minishell_equal_bash "ls | echo -n after without endline"
}

@test "test pipe and echo: echo redirected  > \$file1 | cat" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "echo to file > $file1 | cat
echo $?
cat $file1"
}

@test "test pipe and echo: ls | echo to file > \$file1" {
    file1="$temp_dir/a.txt"
    assert_minishell_equal_bash "ls | echo to file > $file1
echo $?
cat $file1
"
}

@test "test pipe and echo: echo piped | wc | cat" {
    assert_minishell_equal_bash "echo piped | wc | cat"
}

@test "test pipe and echo: ls -l | echo piped | cat" {
    assert_minishell_equal_bash "ls -l | echo piped | cat"
}

@test "test pipe and echo: ls | wc -c | echo to out" {
    assert_minishell_equal_bash "ls | wc -c | echo to out"
}

@test "test pipe and echo: echo piped | echo piped | cat -e" {
    assert_minishell_equal_bash "echo piped | echo piped | cat -e"
}

@test "test pipe and echo: echo piped | wc | echo to out" {
    assert_minishell_equal_bash "echo piped | wc | echo to out"
}

@test "test pipe and echo: uname | echo piped | echo to out" {
    assert_minishell_equal_bash "uname | echo piped | echo to out"
}

@test "test pipe and echo: echo piped | echo piped | echo to out" {
    assert_minishell_equal_bash "echo piped | echo piped | echo to out"
}

@test "test pipe and echo: echo to file > \$file1 | echo to file > \$file2 | echo to file > \$file3" {
    file1="$temp_dir/a.txt"
    file2="$temp_dir/b.txt"
    file3="$temp_dir/c.txt"
    assert_minishell_equal_bash "echo to file > $file1 | echo to file > $file2 | echo to file > $file3
cat $file1
cat $file2
cat $file3
"
}

@test "test simple command expand filename: ls > ./temp_dir/\$LANGUAGE" {
    assert_minishell_equal_bash "
echo \$LANGUAGE
ls > $temp_dir/\$LANGUAGE 
ls $temp_dir
cat < $temp_dir/\$LANGUAGE"
}


@test "test simple command expand filename ambiguous: ls > ./temp_dir/\$VARIABLE_FROM_OUTSIDE" {
    assert_minishell_equal_bash "
echo \$VARIABLE_FROM_OUTSIDE
ls > $temp_dir/\$VARIABLE_FROM_OUTSIDE 
ls $temp_dir
cat < $temp_dir/\$VARIABLE_FROM_OUTSIDE"
}


@test "test simple command simple heredoc: eof\\nsimple heredoc\\neof" {
    assert_minishell_equal_bash_heredoc "cat << eof
simple heredoc
eof
"
}

@test "test simple command heredoc C-d: eof\\nsimple heredoc\\nC-d" {
    assert_minishell_equal_bash_heredoc "cat << eof
simple heredoc
"
}

@test "test simple command simple heredoc: eof\\nsimple\\nheredoc\\neof" {
    assert_minishell_equal_bash_heredoc "cat << eof
simple
heredoc
eof
"
}

@test "test simple command heredoc C-d: eof\\nclose input next line\\nC-d" {
    assert_minishell_equal_bash_heredoc "cat << eof
simple
heredoc
"
}

@test "test simple command simple heredoc: eof\\nwith\\n\\nempty line\\neof" {
    assert_minishell_equal_bash_heredoc "cat << eof
with

empty line
eof
"
}

@test "test simple command heredoc C-d: eof\\n\\n\\nempty line\\n\\nC-d" {
    assert_minishell_equal_bash_heredoc "cat << eof
a
empty line
"
}

@test "test simple command heredoc with expansion: eof\$VARIABLE_FROM_OUTSIDE\\neof" {
    assert_minishell_equal_bash_heredoc "cat << eof
\$VARIABLE_FROM_OUTSIDE
eof
"
}

@test "test simple command heredoc C-d with expansion: eof\$VARIABLE_FROM_OUTSIDE" {
    assert_minishell_equal_bash_heredoc "cat << eof
\$VARIABLE_FROM_OUTSIDE
"
}


@test "test simple command heredoc with expansion: true\\ncat << eof\\n\$?\\neof" {
    assert_minishell_equal_bash_heredoc "true
cat << eof
\$?
eof
"
}

@test "test simple command heredoc with expansion: false\\ncat << echo\\n\$?\\neof" {
    assert_minishell_equal_bash_heredoc "false
cat << eof
\$?
eof
"
}

@test "test heredoc big input: cat << eof\n many lines" {
    assert_minishell_equal_bash "cat << eof | wc
At auctor urna nunc id cursus metus aliquam eleifend mi in nulla posuere sollicitudin aliquam ultrices sagittis orci, a scelerisque purus semper eget duis at. Malesuada pellentesque elit eget gravida?
Id cursus metus aliquam eleifend mi in nulla posuere sollicitudin aliquam ultrices sagittis orci, a scelerisque purus semper eget duis. Enim, sit amet venenatis urna cursus eget nunc scelerisque viverra.
Egestas congue quisque egestas diam in arcu cursus euismod quis viverra nibh cras pulvinar mattis nunc, sed blandit? Ultricies lacus sed turpis tincidunt id aliquet risus feugiat in ante metus.
Pellentesque adipiscing commodo elit, at imperdiet dui accumsan sit amet nulla facilisi morbi? Nisl nunc mi ipsum, faucibus vitae aliquet nec, ullamcorper sit amet risus nullam eget felis eget nunc?
Amet est placerat in egestas erat imperdiet sed euismod nisi porta lorem mollis aliquam ut? Commodo viverra maecenas accumsan, lacus vel facilisis volutpat, est velit egestas dui, id ornare arcu!
Eget dolor morbi non arcu risus, quis varius quam quisque id diam vel quam elementum pulvinar etiam non quam lacus suspendisse faucibus interdum posuere lorem. Vitae purus faucibus ornare suspendisse.
Tempor id eu nisl nunc mi ipsum. Tortor at risus viverra adipiscing at in tellus integer feugiat scelerisque varius morbi enim nunc, faucibus a pellentesque sit amet, porttitor eget dolor!
Amet luctus venenatis, lectus magna fringilla urna, porttitor rhoncus dolor. Volutpat odio facilisis mauris sit amet massa vitae tortor condimentum lacinia quis vel eros donec ac odio tempor orci dapibus.
Hendrerit lectus a molestie lorem ipsum dolor sit amet, consectetur adipiscing elit ut aliquam, purus sit amet luctus venenatis, lectus magna! Sapien, faucibus et molestie ac, feugiat sed lectus vestibulum.
Risus sed vulputate odio ut enim blandit volutpat maecenas volutpat blandit aliquam etiam erat velit, scelerisque in dictum non, consectetur. Adipiscing elit ut aliquam, purus sit amet luctus venenatis, lectus!
Gravida dictum fusce ut placerat orci nulla pellentesque dignissim enim, sit amet venenatis urna cursus eget nunc scelerisque viverra mauris, in aliquam. Ac odio tempor orci dapibus ultrices in iaculis?
Dolor sit amet, consectetur adipiscing elit duis tristique sollicitudin nibh sit amet commodo nulla facilisi nullam vehicula ipsum a arcu. Luctus accumsan tortor posuere ac ut consequat semper viverra nam!
Egestas maecenas pharetra convallis posuere. Accumsan sit amet nulla facilisi morbi tempus iaculis urna, id volutpat lacus laoreet non curabitur gravida arcu ac tortor dignissim convallis aenean et tortor at.
Nulla aliquet porttitor lacus, luctus accumsan tortor posuere. Ornare aenean euismod elementum nisi, quis eleifend quam adipiscing vitae proin sagittis, nisl rhoncus mattis rhoncus, urna neque viverra justo, nec ultrices.
Felis eget nunc lobortis mattis aliquam faucibus purus in massa tempor nec feugiat. Suscipit tellus mauris a diam maecenas sed enim ut sem viverra aliquet eget sit amet tellus cras.
Purus, in mollis nunc sed id semper risus in hendrerit gravida rutrum quisque non tellus orci, ac. Ultrices vitae, auctor eu augue ut lectus arcu, bibendum at varius vel, pharetra!
A pellentesque sit amet, porttitor eget dolor morbi non? Et netus et malesuada fames ac turpis egestas integer eget aliquet nibh praesent tristique magna sit amet purus gravida quis blandit.
Est ante in nibh mauris. Libero volutpat sed cras ornare arcu dui vivamus arcu felis, bibendum ut tristique et, egestas quis ipsum suspendisse ultrices gravida dictum fusce ut placerat orci.
Accumsan sit amet nulla facilisi. Sed vulputate odio ut enim blandit volutpat maecenas volutpat blandit aliquam etiam erat velit, scelerisque in dictum non, consectetur a erat nam at lectus urna.
Duis convallis convallis tellus, id interdum. Curabitur vitae nunc sed velit dignissim sodales ut eu sem integer vitae justo eget magna fermentum iaculis eu non diam phasellus vestibulum lorem sed.
Sem fringilla ut morbi tincidunt augue interdum velit euismod in pellentesque massa placerat duis ultricies lacus sed turpis tincidunt id aliquet risus feugiat in. Sit amet cursus sit amet, dictum.
Pharetra, massa massa ultricies mi, quis hendrerit dolor magna! Bibendum est ultricies integer quis auctor elit sed vulputate mi sit amet mauris commodo quis imperdiet massa tincidunt nunc pulvinar sapien.
Libero volutpat sed cras ornare arcu. Elementum integer enim neque, volutpat ac tincidunt vitae, semper quis lectus nulla at volutpat diam ut venenatis tellus in metus vulputate eu scelerisque felis?
Mauris a diam maecenas sed enim ut sem viverra aliquet eget sit amet tellus cras adipiscing enim eu turpis egestas pretium. Duis ultricies lacus sed turpis tincidunt id aliquet risus.
Tellus molestie nunc, non blandit massa enim nec dui nunc mattis enim ut tellus elementum sagittis vitae et leo duis ut diam. Neque sodales ut etiam sit amet nisl purus?
Turpis massa tincidunt dui ut ornare lectus sit amet est placerat in egestas erat imperdiet sed euismod nisi porta? Tristique sollicitudin nibh sit amet commodo nulla facilisi nullam vehicula ipsum?
Tellus in metus vulputate eu scelerisque felis imperdiet proin fermentum leo vel orci porta non pulvinar neque laoreet suspendisse interdum consectetur libero, id faucibus nisl? Dictum at tempor commodo, ullamcorper.
Tempus egestas sed sed risus pretium quam vulputate dignissim suspendisse in est ante in nibh mauris, cursus mattis molestie a, iaculis at! Pharetra pharetra, massa massa ultricies mi, quis hendrerit?
Cras tincidunt lobortis feugiat vivamus at augue eget arcu dictum varius duis at consectetur lorem donec massa. Eget gravida cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus.
Diam ut venenatis tellus in metus vulputate eu scelerisque felis imperdiet proin fermentum leo vel. Quisque sagittis, purus sit amet volutpat consequat, mauris nunc congue nisi, vitae suscipit tellus mauris?
Molestie ac, feugiat sed lectus vestibulum mattis ullamcorper velit sed ullamcorper morbi tincidunt ornare massa, eget egestas. In nibh mauris, cursus mattis molestie a, iaculis at erat pellentesque adipiscing commodo.
Posuere morbi leo urna, molestie at elementum eu, facilisis sed odio morbi quis! Amet nisl purus, in mollis nunc sed id semper risus in hendrerit gravida rutrum quisque non tellus.
Ullamcorper dignissim cras tincidunt lobortis feugiat vivamus at augue eget arcu dictum varius duis at consectetur lorem donec massa sapien. Duis at tellus at urna condimentum mattis pellentesque id nibh?
Sem integer vitae justo eget magna fermentum iaculis eu non diam phasellus vestibulum lorem sed risus ultricies tristique nulla aliquet enim tortor, at auctor. Aliquam faucibus purus in massa tempor?
Dictum at tempor commodo, ullamcorper a lacus vestibulum sed arcu non odio euismod. Amet risus nullam eget felis eget nunc lobortis mattis aliquam faucibus purus in massa tempor nec feugiat.
Egestas diam in arcu cursus euismod quis viverra nibh cras pulvinar mattis? Quis vel eros donec ac odio tempor orci dapibus ultrices in iaculis nunc sed augue lacus, viverra vitae!
Ultrices in iaculis nunc sed augue lacus, viverra vitae congue eu, consequat ac felis donec et odio pellentesque diam volutpat commodo sed egestas. Id aliquet lectus proin nibh nisl, condimentum!
Sed augue lacus, viverra vitae congue eu, consequat ac felis donec et odio. Morbi non arcu risus, quis varius quam quisque id diam vel quam elementum pulvinar etiam non quam!
Viverra nibh cras pulvinar mattis. Libero justo, laoreet sit amet cursus sit amet, dictum sit amet justo donec enim diam, vulputate ut pharetra sit amet, aliquam id diam maecenas ultricies!
Eu non diam phasellus vestibulum lorem sed risus ultricies tristique nulla aliquet enim tortor, at auctor urna nunc id cursus? Molestie lorem ipsum dolor sit amet, consectetur adipiscing elit ut?
Aliquet bibendum enim, facilisis gravida neque convallis a cras? Massa enim nec dui nunc mattis enim ut tellus elementum sagittis vitae et leo duis ut diam quam nulla porttitor massa!
Pellentesque elit eget gravida cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus mauris vitae ultricies leo integer malesuada nunc vel risus. Aenean euismod elementum nisi, quis!
Lobortis elementum, nibh tellus molestie nunc, non blandit massa enim nec dui nunc mattis enim ut tellus elementum sagittis vitae. Mus mauris vitae ultricies leo integer malesuada nunc vel risus?
Quam viverra orci sagittis eu volutpat odio facilisis mauris sit amet massa vitae tortor condimentum lacinia quis vel eros. Fermentum leo vel orci porta non pulvinar neque laoreet suspendisse interdum!
Eu non diam phasellus vestibulum lorem sed risus ultricies tristique nulla aliquet enim tortor, at auctor urna nunc id cursus metus aliquam! Augue ut lectus arcu, bibendum at varius vel.
Varius quam quisque id diam vel quam elementum pulvinar etiam non quam lacus suspendisse? Luctus venenatis, lectus magna fringilla urna, porttitor rhoncus dolor purus non enim praesent elementum facilisis leo! e diam volutpat commodo sed egestas egestas fringilla phasellus faucibus scelerisque eleifend donec pretium vulputate sapien nec sagittis aliquam malesuada bibendum. Dictum sit amet justo donec enim diam, vulputate ut?
Risus nec feugiat in fermentum posuere urna nec tincidunt praesent semper feugiat nibh sed pulvinar proin gravida hendrerit lectus a molestie? Molestie nunc, non blandit massa enim nec dui nunc.
Vivamus arcu felis, bibendum ut tristique et, egestas quis ipsum suspendisse ultrices gravida! Cursus vitae congue mauris rhoncus aenean vel elit scelerisque mauris pellentesque pulvinar pellentesque habitant morbi tristique senectus!
Ultrices sagittis orci, a scelerisque. Felis eget nunc lobortis mattis aliquam faucibus purus in massa tempor nec feugiat nisl pretium fusce id velit ut tortor pretium viverra suspendisse potenti nullam.
Suspendisse potenti nullam ac tortor vitae purus faucibus ornare suspendisse sed nisi lacus, sed viverra tellus in hac habitasse platea dictumst vestibulum rhoncus. Nibh sit amet commodo nulla facilisi nullam? nunc pulvinar sapien et ligula ullamcorper malesuada proin libero nunc! Amet, aliquam id diam maecenas ultricies mi eget mauris pharetra et ultrices neque ornare aenean euismod elementum nisi?
Tincidunt dui ut ornare lectus sit amet est placerat in egestas erat imperdiet sed euismod nisi porta lorem mollis aliquam ut! Donec massa sapien, faucibus et molestie ac, feugiat sed.
Praesent semper feugiat nibh sed pulvinar proin gravida hendrerit lectus a molestie lorem! Justo eget magna fermentum iaculis eu non diam phasellus vestibulum lorem sed risus ultricies tristique nulla aliquet? non tellus orci, ac auctor augue mauris augue neque, gravida in fermentum et, sollicitudin ac orci! In fermentum et, sollicitudin ac orci phasellus egestas tellus rutrum tellus pellentesque!
In vitae turpis massa sed elementum tempus egestas sed. Dolor sit amet, consectetur adipiscing elit ut aliquam, purus sit amet luctus venenatis, lectus magna fringilla urna, porttitor rhoncus dolor purus.
Eget gravida cum sociis natoque penatibus? Augue ut lectus arcu, bibendum at varius vel, pharetra vel turpis nunc eget lorem dolor, sed viverra ipsum nunc aliquet bibendum enim, facilisis gravida.
Eget nunc scelerisque viverra mauris, in aliquam sem. Feugiat vivamus at augue eget arcu dictum varius duis at consectetur lorem donec massa sapien, faucibus et molestie ac, feugiat sed lectus.
Elit duis tristique sollicitudin nibh sit amet? Elementum curabitur vitae nunc sed velit dignissim sodales ut eu sem integer vitae justo eget magna fermentum iaculis eu non diam phasellus vestibulum.
Egestas integer eget aliquet nibh praesent tristique magna sit amet purus gravida quis blandit turpis cursus in hac habitasse platea dictumst quisque sagittis, purus sit. In mollis nunc sed id?
Congue nisi, vitae suscipit tellus mauris a diam maecenas sed enim ut sem viverra aliquet eget sit amet tellus cras adipiscing enim! Egestas maecenas pharetra convallis posuere morbi leo urna.
Diam quis enim lobortis scelerisque fermentum dui faucibus? Elit ullamcorper dignissim cras tincidunt lobortis feugiat vivamus at augue eget arcu dictum varius duis at consectetur lorem donec massa sapien, faucibus.
Morbi tempus iaculis urna, id volutpat lacus laoreet non curabitur gravida arcu ac tortor dignissim convallis aenean et tortor. Aliquam sem fringilla ut morbi tincidunt augue interdum velit euismod in.
A molestie lorem ipsum dolor sit amet, consectetur adipiscing elit ut aliquam, purus sit amet? Tellus at urna condimentum mattis pellentesque id nibh tortor, id aliquet lectus proin nibh nisl.
Vitae nunc sed velit dignissim sodales ut eu sem integer vitae justo eget magna fermentum iaculis eu non diam phasellus vestibulum lorem sed. Enim nulla aliquet porttitor lacus, luctus accumsan.
Sed sed risus pretium quam vulputate. Risus commodo viverra maecenas accumsan, lacus vel facilisis volutpat, est velit egestas dui, id ornare arcu odio ut sem nulla pharetra diam sit amet?
Sit amet, mattis vulputate enim nulla aliquet porttitor? Facilisi morbi tempus iaculis urna, id volutpat lacus laoreet non curabitur gravida arcu ac tortor dignissim convallis aenean et tortor at risus?
Sit amet volutpat consequat, mauris nunc congue nisi, vitae suscipit tellus mauris. Morbi tincidunt ornare massa, eget egestas purus viverra accumsan in nisl nisi, scelerisque eu ultrices vitae, auctor eu.
Eget mauris pharetra et ultrices neque ornare aenean euismod elementum nisi, quis. Ultrices mi tempus imperdiet nulla malesuada pellentesque elit eget gravida cum sociis natoque penatibus et magnis dis parturient.
Amet, consectetur adipiscing elit ut. Nam libero justo, laoreet sit amet cursus sit amet, dictum sit amet justo donec enim diam, vulputate ut pharetra sit amet, aliquam id diam maecenas!
Ac, feugiat sed lectus vestibulum mattis ullamcorper velit. Cras adipiscing enim eu turpis egestas pretium aenean pharetra, magna ac placerat vestibulum, lectus mauris ultrices eros, in cursus turpis massa tincidunt.
Tortor dignissim convallis aenean et tortor at risus viverra adipiscing at in tellus integer feugiat scelerisque. Odio facilisis mauris sit amet massa vitae tortor condimentum lacinia quis vel eros donec?
Quis viverra nibh cras pulvinar mattis nunc, sed blandit libero volutpat sed cras ornare arcu. Feugiat vivamus at augue eget arcu dictum varius duis at consectetur lorem donec massa sapien.
Tortor, id aliquet lectus proin nibh nisl, condimentum id! Lacus vel facilisis volutpat, est velit egestas dui, id ornare arcu odio ut sem nulla pharetra diam sit amet nisl suscipit?
Sed cras ornare arcu dui vivamus arcu felis, bibendum ut tristique et, egestas quis ipsum suspendisse ultrices gravida dictum! Scelerisque purus semper eget duis at tellus at urna condimentum mattis?
Nunc sed augue lacus, viverra vitae congue eu, consequat ac felis donec et odio pellentesque diam volutpat commodo sed egestas egestas fringilla phasellus? Sit amet est placerat in egestas erat.
Eget magna fermentum iaculis eu non diam phasellus vestibulum lorem sed risus ultricies? Turpis egestas maecenas pharetra convallis posuere morbi leo urna, molestie at elementum eu, facilisis sed odio morbi!
Aliquam id diam maecenas ultricies mi eget mauris pharetra et ultrices neque ornare. Consequat id porta nibh venenatis cras sed felis eget velit aliquet sagittis id consectetur purus ut faucibus?
Non, consectetur a erat nam at lectus urna duis convallis convallis tellus, id interdum velit laoreet id donec ultrices tincidunt arcu, non sodales neque. Lectus quam id leo in vitae!
Lacus suspendisse faucibus interdum posuere lorem ipsum. Viverra tellus in hac habitasse platea dictumst vestibulum rhoncus est pellentesque elit ullamcorper dignissim cras tincidunt lobortis feugiat vivamus at augue eget arcu.
Congue quisque egestas diam in arcu cursus euismod! Molestie ac, feugiat sed lectus vestibulum mattis ullamcorper velit sed ullamcorper morbi tincidunt ornare massa, eget egestas purus viverra accumsan in nisl?
Venenatis urna cursus eget nunc scelerisque viverra mauris, in aliquam sem fringilla ut morbi tincidunt augue interdum velit. Volutpat blandit aliquam etiam erat velit, scelerisque in dictum non, consectetur a?
Lacus vel facilisis volutpat, est velit egestas. Amet, porttitor eget dolor morbi non arcu risus, quis varius quam quisque id diam vel quam elementum pulvinar etiam non quam lacus suspendisse.
Donec et odio pellentesque diam volutpat commodo sed egestas egestas fringilla phasellus faucibus scelerisque eleifend donec pretium vulputate sapien nec sagittis aliquam malesuada bibendum! In est ante in nibh mauris!
Sit amet mauris commodo quis? Interdum consectetur libero, id faucibus nisl tincidunt eget nullam non nisi est, sit amet facilisis magna etiam tempor, orci eu lobortis elementum, nibh tellus molestie!
Tortor at risus viverra adipiscing at in tellus integer feugiat scelerisque. Imperdiet sed euismod nisi porta lorem mollis aliquam ut porttitor leo a diam sollicitudin tempor id eu nisl nunc.
Eget dolor morbi non arcu risus, quis varius quam quisque id diam vel! Eget velit aliquet sagittis id consectetur purus ut faucibus pulvinar elementum integer enim neque, volutpat ac tincidunt.
eof
"
}

@test "test command and: ls && uname" {
    assert_minishell_equal_bash "ls && uname"
}

@test "test command and: false && uname" {
    assert_minishell_equal_bash "false && uname"
}

@test "test command and: true && uname" {
    assert_minishell_equal_bash "true && uname"
}

@test "test command and: \" \" && uname" {
    assert_minishell_equal_bash "\" \" && uname"
}

@test "test command and: uname && \" \"" {
    assert_minishell_equal_bash "uname && \" \""
}

@test "test command and: false && echo \$?" {
    assert_minishell_equal_bash "false && echo $?"
}

@test "test command and: true && echo \$?" {
    assert_minishell_equal_bash "true && echo $?"
}

@test "test command and: echo ok && echo \$?" {
    assert_minishell_equal_bash "echo ok && echo $?"
}

@test "test command and: ls || uname" {
    assert_minishell_equal_bash "ls || uname"
}

@test "test command and: false || uname" {
    assert_minishell_equal_bash "false || uname"
}

@test "test command and: true || uname" {
    assert_minishell_equal_bash "true || uname"
}

@test "test command and: \" \" || uname" {
    assert_minishell_equal_bash "\" \" || uname"
}

@test "test command and: uname || \" \"" {
    assert_minishell_equal_bash "uname || \" \""
}

@test "test command and: false || echo \$?" {
    assert_minishell_equal_bash "false || echo $?"
}

@test "test command and: true || echo \$?" {
    assert_minishell_equal_bash "true || echo $?"
}

@test "test command and: echo ok || echo \$?" {
    assert_minishell_equal_bash "echo ok || echo $?"
}

@test "test command pipe with and: ls && uname | cat - e" {
    assert_minishell_equal_bash "ls && uname | cat -e"
}

@test "test command pipe with and: ls | cat - e && uname" {
    assert_minishell_equal_bash "ls | cat - e && uname"
}

@test "test command pipe with and: false && uname | cat - e" {
    assert_minishell_equal_bash "false && uname | cat - e"
}

@test "test command pipe with and: false | cat - e && uname" {
    assert_minishell_equal_bash "false | cat - e && uname"
}

@test "test command pipe with and: true && uname | cat - e" {
    assert_minishell_equal_bash "true && uname | cat - e"
}

@test "test command pipe with and: true | cat - e && uname" {
    assert_minishell_equal_bash "true | cat - e && uname"
}

@test "test command pipe with and: \" \" | cat - e && uname" {
    assert_minishell_equal_bash "\" \" | cat - e && uname"
}

@test "test command pipe with and: uname && \" \"" {
    assert_minishell_equal_bash "uname && \" \"" 
}

@test "test command pipe with or: ls || uname | cat - e" {
    assert_minishell_equal_bash "ls || uname | cat -e"
}

@test "test command pipe with or: ls | cat - e || uname" {
    assert_minishell_equal_bash "ls | cat - e || uname"
}

@test "test command pipe with or: false || uname | cat - e" {
    assert_minishell_equal_bash "false || uname | cat - e"
}

@test "test command pipe with or: false | cat - e || uname" {
    assert_minishell_equal_bash "false | cat - e || uname"
}

@test "test command pipe with or: true || uname | cat - e" {
    assert_minishell_equal_bash "true || uname | cat - e"
}

@test "test command pipe with or: true | cat - e || uname" {
    assert_minishell_equal_bash "true | cat - e || uname"
}

@test "test command pipe with or: \" \" | cat - e || uname" {
    assert_minishell_equal_bash "\" \" | cat - e || uname"
}

@test "test command pipe with or: uname || \" \"" {
    assert_minishell_equal_bash "uname || \" \"" 
}

@test "test expand unknown variable: \$DOES_NOT_EXIST" {
    assert_minishell_equal_bash "\$DOES_NOT_EXIST$$
"
}

@test "test expand file unknown variable: ls > \$DOES_NOT_EXIST" {
    assert_minishell_equal_bash "ls > \$DOES_NOT_EXIST$$
"
}

@test "test expand file unknown variable: cat < \$DOES_NOT_EXIST" {
    assert_minishell_equal_bash "cat < \$DOES_NOT_EXIST$$
"
}

@test "test export update env variable value" {
    assert_minishell_equal_bash "echo \$HELLO
export HELLO=123
echo \$HELLO
export HELLO=\"beautiful world\"
echo \$HELLO
"
}

@test "invalid command followed by empty input keeps the exit code" {
    assert_minishell_equal_bash "doesntexist

echo \$?
"
}

@test "invalid command followed by expansion of non set variable keeps the exit code" {
    assert_minishell_equal_bash "doesntexist
$EMPTY
echo \$?
"
}

@test "test_syntax: |" {
    assert_minishell_equal_bash "
|
"
}

@test "test_syntax: | ls" {
    assert_minishell_equal_bash "
| ls
"
}

@test "test_syntax: | |" {
    assert_minishell_equal_bash "
| |
"
}

@test "test_syntax: | \$" {
    assert_minishell_equal_bash "
| \$
"
}

@test "test_syntax: | >" {
    assert_minishell_equal_bash "
| >
"
}

@test "test_syntax: >" {
    assert_minishell_equal_bash "
>
"
}

@test "test_syntax: >>" {
    assert_minishell_equal_bash "
>>
"
}

@test "test_syntax: >>>" {
    assert_minishell_equal_bash "
>>>
"
}

@test "test_syntax: <" {
    assert_minishell_equal_bash "
<
"
}

@test "test_syntax: <<" {
    assert_minishell_equal_bash "
<<
"
}

@test "test_syntax: ls <" {
    assert_minishell_equal_bash "
ls <
"
}

@test "test_syntax: cat  < | ls" {
    assert_minishell_equal_bash "
cat  < | ls
"
}

@test "test_syntax: ls | >" {
    assert_minishell_equal_bash "
ls | >
"
}

@test "test_syntax: ls | > >>" {
    assert_minishell_equal_bash "
ls | > >>
"
}

@test "test_syntax: ls | < |" {
    assert_minishell_equal_bash "
ls | < |
"
}

@test "test_syntax: ls |   |" {
    assert_minishell_equal_bash "
ls |   |
"
}

@test "test_syntax: ls |  \"|\"" {
    assert_minishell_equal_bash "
ls |  \"|\"
"
}

@test "test_syntax: ls | ls | |" {
    assert_minishell_equal_bash "
ls | ls | | 
"
}

@test "test_syntax: ls | ls | &&" {
    assert_minishell_equal_bash "
ls | ls | &&
"
}


@test "test_syntax: ls | ls | ||" {
    assert_minishell_equal_bash "
ls | ls | ||
"
}

@test "test_syntax: &&" {
    assert_minishell_equal_bash "
&&
"
}

@test "test_syntax: && ls" {
    assert_minishell_equal_bash "
&& ls
"
}

@test "test_syntax: ||" {
    assert_minishell_equal_bash "
||
"
}

@test "test_syntax: || ls" {
    assert_minishell_equal_bash "
|| ls
"
}

@test "test_invocations: \$PWD" {
    assert_minishell_equal_bash "
\$PWD
"
}

@test "test_invocations: \$EMPTY" {
    assert_minishell_equal_bash "
\$EMPTY
"
}

@test "test_invocations: \$EMPTY echo hi" {
    assert_minishell_equal_bash "
\$EMPTY echo hi
"
}

@test "test_invocations: ./missing.out" {
    assert_minishell_equal_bash "
./missing.out
"
}

@test "test_invocations: missing.out" {
    assert_minishell_equal_bash "
missing.out
"
}

@test "test_invocations: \"aaa\"" {
    assert_minishell_equal_bash "
\"aaa\"
"
}

@test "test_invocations: \$" {
    assert_minishell_equal_bash "
\$
"
}

@test "test_invocations: \$?" {
    assert_minishell_equal_bash "
\$?
"
}





# @test "test export no args" {
#     assert_minishell_equal_bash "
# echo \$BASH_FUNC_bats_readlinkf%%
# unset _
# unset BATS_BEGIN_CODE_QUOTE
# export
# "
# }


# # keep track of OLDPWD
# cd obj
# echo $PWD $OLDPWD

# #invalid command, followed by empty variable, should clear the exit code
 # doesntexist
# $EMPTY
# echo $?

# # Check if there isn't a zombie process called `cat`
# echo "hi" | cat | cat | cat | cat | cat | cat | cat
# ps -a

# # Should skip the empty argument, and print hello after spaces
# echo - "" "  " hello

# # Neither of these unsets should break the shell, and you should still be able to call `/bin/ls`
# unset USER
# unset PATH
# unset PWD
# /bin/ls

# # This should not change the current directory
# cd .. hi

# # Empty `cd` moves to home
# cd"

