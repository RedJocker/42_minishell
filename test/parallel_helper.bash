#!/usr/bin/env bash
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    parallel_helper.bash                               :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/26 03:15:43 by dande-je          #+#    #+#              #
#    Updated: 2024/10/28 03:28:23 by dande-je         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Get the absolute path to the project root directory
export PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
export MINISHELL_PATH="$PROJECT_ROOT/minishell"

# This ensures thread-safe file operations
setup_file() {
    export TEST_TEMP_DIR="./test/temp_${PPID}_${RANDOM}"
    mkdir -p "$TEST_TEMP_DIR"
}

teardown_file() {
	# Cleanup any remaining processes
    rm -rf "$TEST_TEMP_DIR"
}

setup() {
    # Create unique temporary directory for each test
    export TEST_CASE_DIR="$TEST_TEMP_DIR/case_${BATS_TEST_NUMBER}_${RANDOM}"
    mkdir -p "$TEST_CASE_DIR"
    cd "$TEST_CASE_DIR"
}

teardown() {
	# Cleanup test-specific processes
    cd "$PROJECT_ROOT"
    rm -rf "$TEST_CASE_DIR"
}

bash_execute() {
	VARIABLE_FROM_OUTSIDE_MORE_SPACES="abc    def" \
	VARIABLE_FROM_OUTSIDE="abc def" \
	LANGUAGE="en" \
	PS1='RedWillShell$ ' \
	bash --norc -i <<< "$@"
}

minishell_execute() {
    VARIABLE_FROM_OUTSIDE_MORE_SPACES="abc    def" \
    VARIABLE_FROM_OUTSIDE="abc def" \
    LANGUAGE="en" \
    PS1='RedWillShell$ ' \
    "$MINISHELL_PATH" <<< "$@"
}

minishell_leak_check() {
	VARIABLE_FROM_OUTSIDE_MORE_SPACES="abc    def" \
	VARIABLE_FROM_OUTSIDE="abc def" \
	LANGUAGE="en" \
	PS1='RedWillShell$ ' \
    valgrind --leak-check=full \
        -s \
        --show-reachable=yes \
        --errors-for-leak-kinds=all \
        --error-exitcode=33 \
        --track-origins=yes \
        --track-fds=yes \
        --suppressions="$PROJECT_ROOT/mini.supp" \
        "$MINISHELL_PATH" <<< "$@"
}

assert_minishell_equal_bash() {
    run bash_execute "$@"
    local bash_status=$status
	local bash_output=$(echo "$output" | sed 's/bash: /minishell: /g')

    run minishell_execute "$@"

    if ! [[ $bash_output == $output ]]; then
        echo "$bash_output" > "./test/bash_"$BATS_TEST_NAME".txt"
        echo "$output" > "./test/mini_"$BATS_TEST_NAME".txt"
		echo -e "===> bash_output:\n<$bash_output>\n\n===> minishell_output:\n<$output>"
        echo -e "\ndiff:\n====================\n"
        diff "./test/bash_"$BATS_TEST_NAME".txt" "./test/mini_"$BATS_TEST_NAME".txt" || true
        echo -e "\n====================\n"
        false
    fi

    if ! [[ $bash_status == $status ]]; then
        echo -e "===> bash_status: $bash_status\nminishell_status: $status"
        false
    fi

    run minishell_leak_check "$@"

    if (( status == 33 )); then
	echo -e "VALGRIND ERROR:\n$output"
	false
    fi
}

assert_minishell_equal_bash_heredoc() {
    run bash_execute "$@"

    local bash_status=$status
    local bash_output=$output

    local bash_out_norm=$(awk 'NR > 2 && /here-document at line/ { gsub(/at line [0-9]+ /, "", $0); print $0} !/here-document/ { print $0}' <<< "$output")
	local bash_output_heredoc=$(echo "$bash_out_norm" | sed 's/bash: /minishell: /g')

    run minishell_execute "$@"

    if ! [[ $bash_output_heredoc == $output ]]; then
        echo "$bash_output_heredoc" > "./test/bash_"$BATS_TEST_NAME".txt"
        echo "$output" > "./test/mini_"$BATS_TEST_NAME".txt"
		echo -e "===> bash_output:\n<$bash_output_heredoc>\n\n===> minishell_output:\n<$output>"
        echo -e "\ndiff:\n====================\n"
        diff "./test/bash_"$BATS_TEST_NAME".txt" "./"$TEST_DIR"/mini_"$BATS_TEST_NAME".txt" || true
        echo -e "\n====================\n"
		false
    fi

    if ! [[ $bash_status == $status ]]; then
		echo -e "===> bash_status: $bash_status\nminishell_status: $status"
		false
    fi

    run minishell_leak_check "$@"

    if (( status == 33 )); then
	echo -e "VALGRIND ERROR:\n$output"
	false
    fi
}
