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

@test "test cd: cd \"\$temp_dir2\" \n pwd \n cd . \n pwd \n cd .. \n pwd" {
    temp_dir2="$TEST_CASE_DIR/temp"
    assert_minishell_equal_bash "
mkdir -p \"$temp_dir2\"
cd \"$temp_dir2\"
pwd
cd .
pwd
cd ..
pwd"
}

@test "test cd: cd \"\$temp_dir2\" \n echo \$PWD \n echo \$OLD_PWD \n cd . \n echo \$PWD \n echo \$OLD_PWD \n cd .. \n echo \$PWD \n echo \$OLD_PWD" {

    temp_dir2="$TEST_CASE_DIR/temp"
    assert_minishell_equal_bash "
mkdir -p \"$temp_dir2\"
cd \"$temp_dir2\"
echo \$PWD
echo \$OLDPWD
cd .
echo \$PWD
echo \$OLDPWD
cd ..
echo \$PWD 
echo \$OLDPWD"
}
