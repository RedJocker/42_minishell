#!/usr/bin/env bats

setup() {
    load ../setup_core
}

@test "test wildcards: ls *" {
    assert_minishell_equal_bash "ls *
/usr/bin/printf \"%s\n\" * | sort
export ARG=\"*\"
/usr/bin/printf \"%s\n\" $ARG | sort
ls *
ls 'test'*
ls \"test\"*
touch \"   \"
ls * | grep \"   \"
touch \"a\" \"aa\"
/usr/bin/printf \"%s\n\" *a | sort
touch \"a\" \"aa\" \"aaa\"
/usr/bin/printf \"%s\n\" *a*a | sort
" 
}
#
#
#
#
#
#
# touch \"a\" \"aa\" \"aaa\"
# /usr/bin/printf \"%s\n\" *a*a | sort
# rm \"a\" \"aa\" \"aaa\"
#
# /usr/bin/printf \"%s\n\" \"Mak\"'e'*'*' | sort
#
# /usr/bin/printf \"%s\n\" \"Mak\"'e'** | sort
#
# /usr/bin/printf \"%s\n\" ***\"Mak\"'e'** | sort
#
# /usr/bin/printf \"%s\n\" \".\"* | sort
#
# /usr/bin/printf \"%s\n\" *\".\" | sort
#
# /usr/bin/printf \"%s\n\" *\".\"* | sort
#
# /usr/bin/printf \"%s\n\" \"*\" | sort
#
# /usr/bin/printf \"%s\n\" '*' | sort
#
# /usr/bin/printf \"%s\n\" Minishe*l | sort
#
# /usr/bin/printf \"%s\n\" minishe*l | sort
#
# touch minishelxxxl
# /usr/bin/printf \"%s\n\" minishe*l | sort
# /usr/bin/printf \"%s\n\" minishel*l | sort
# /usr/bin/printf \"%s\n\" minishelxxx*l | sort
# /usr/bin/printf \"%s\n\" minishel*xxx*l | sort
# rm minishelxxxl
#
# touch tmp_out
# /bin/echo 42 >tmp_o*t*
# rm -f \"tmp_out*\"
#
# touch tmp_out1
# touch tmp_out2
# /bin/echo 42 >tmp_out*
# rm -f \"tmp_out*\"
#
# export test=\" * \"
# touch \"$USER * ?eHallo\"
# /bin/echo \"$USER \"*\" ?e\"*
# rm -f \"$USER * ?eHallo\"
