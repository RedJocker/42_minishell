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

@test "test export: export" {
    assert_minishell_equal_bash "export | grep -v -i 'bats' | grep -v '}' | grep -v '_=' | grep -v '$'"
}

@test "test export: export aaa" {
    assert_minishell_equal_bash "export aaa
export | grep -v -i 'bats' | grep -v '}' | grep -v '_=' | grep -v '$'"
}

@test "test export: export aaa=test" {
    assert_minishell_equal_bash "export aaa=test
export | grep -v -i 'bats' | grep -v '}' | grep -v '_=' | grep -v '$'"
}

@test "test export: export aaa=\"test test\"" {
    assert_minishell_equal_bash "export aaa=\"test test\"
export | grep -v -i 'bats' | grep -v '}' | grep -v '_=' | grep -v '$'"
}

@test "test export: export aaa \n export aaa=test" {
    assert_minishell_equal_bash "export aaa
export | grep -v -i 'bats' | grep -v '}' | grep -v '_=' | grep -v '$'
export aaa=test
export | grep -v -i 'bats' | grep -v '}' | grep -v '_=' | grep -v '$'"
}

@test "test export: export -aaa" {
    assert_minishell_equal_bash "export -aaa
export | grep -v -i 'bats' | grep -v '}' | grep -v '_=' | grep -v '$'"
}

@test "test export : export 1aaa" {
    assert_minishell_equal_bash "export 1aaa
export | grep -v -i 'bats' | grep -v '}' | grep -v '_=' | grep -v '$'"
}

@test "test export: 1aaa=test" {
    assert_minishell_equal_bash "export 1aaa=test
export | grep -v -i 'bats' | grep -v '}' | grep -v '_=' | grep -v '$'"
}
