#!/usr/bin/env bats
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    builtins.bats                                      :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/26 03:16:58 by dande-je          #+#    #+#              #
#    Updated: 2024/10/26 03:16:58 by dande-je         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

setup() {
    load parallel_helper
}

@test "test wildcards: ls *" {
    assert_minishell_equal_bash "ls *"
}

@test "test wildcards: echo *" {
    assert_minishell_equal_bash "echo *"
}

@test "test wildcards ordering: touch xxxx#ab xxx%ab xxx.ab xxxcd xxx#ef xxx%ef xxx.ef\n echo *" {
    assert_minishell_equal_bash "
cd $temp_dir
touch xxx#ab xxx%ab xxx.ab xxxcd xxx#ef xxx%ef xxx.ef
ls
echo *"
}

@test "test wildcards ordering: touch ABC ABc AbC aBC Abc aBc abC abc \n echo *" {
    assert_minishell_equal_bash "
cd $temp_dir
touch ABC ABc AbC aBC Abc aBc abC abc 
ls
echo *"
}

@test "test wildcards: ls > *" {
    assert_minishell_equal_bash "
cd $temp_dir
touch a b c
ls > *"
}

@test "test wildcards: echo > *" {
    assert_minishell_equal_bash "
cd $temp_dir
touch a b c
echo > *"
}

@test "test wildcards: ls \"*\"" {
    assert_minishell_equal_bash "ls \"*\"
"
}

@test "test wildcards: echo \"*\"" {
    assert_minishell_equal_bash "
echo \"*\""
}

@test "test wildcards: ls > '*'" {
    assert_minishell_equal_bash "
cd $temp_dir
touch a b c
ls > '*'"
}

@test "test wildcards: echo > '*'" {
    assert_minishell_equal_bash "
cd $temp_dir
touch a b c
echo > '*'"
}
