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

@test "export" {
    assert_minishell_equal_bash "export | grep -v -i 'bats' | grep -v '}' | grep -v '_=' | grep -v '$'"
}

@test "export key(only with name of key)" {
    assert_minishell_equal_bash "export aaa
export | grep -v -i 'bats' | grep -v '}' | grep -v '_=' | grep -v '$'"
}

@test "export key and value" {
    assert_minishell_equal_bash "export aaa=test
export | grep -v -i 'bats' | grep -v '}' | grep -v '_=' | grep -v '$'"
}

@test "export key and value(with spaces)" {
    assert_minishell_equal_bash "export aaa=\"test test\"
export | grep -v -i 'bats' | grep -v '}' | grep -v '_=' | grep -v '$'"
}

@test "export key(update value)" {
    assert_minishell_equal_bash "export aaa
export | grep -v -i 'bats' | grep -v '}' | grep -v '_=' | grep -v '$'
export aaa=test
export | grep -v -i 'bats' | grep -v '}' | grep -v '_=' | grep -v '$'"
}

@test "export invalid key(-key)" {
    assert_minishell_equal_bash "export -aaa"
}

