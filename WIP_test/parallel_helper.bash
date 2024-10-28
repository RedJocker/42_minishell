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

export TEST_TIMEOUT=10

# Get the absolute path to the project root directory
export PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
export MINISHELL_PATH="$PROJECT_ROOT/minishell"

# This ensures thread-safe file operations
setup_file() {
	# export TEST_TEMP_DIR="./test/temp_$$"
    # Use PID in temporary files to avoid conflicts
    rm -f ./WIP_test/bash_*.txt > /dev/null || true
    rm -f ./WIP_test/mini_*.txt > /dev/null || true
    export TEST_TEMP_DIR="./WIP_test/temp_${PPID}_${RANDOM}"
    mkdir -p "$TEST_TEMP_DIR"
    # Use unique files for each test process
	#    export BASH_OUTPUT_FILE="$TEST_TEMP_DIR/bash_${BATS_TEST_NAME}_${PPID}.txt"
	#    export MINI_OUTPUT_FILE="$TEST_TEMP_DIR/mini_${BATS_TEST_NAME}_${PPID}.txt"
	# export TEST_PIDS_FILE="$TEST_TEMP_DIR/test_pids_${PPID}.txt"
	#    > "$TEST_PIDS_FILE"
}

teardown_file() {
	# Cleanup any remaining processes
    if [[ -f "$TEST_PIDS_FILE" ]]; then
        while read -r pid; do
            pkill -P "$pid" 2>/dev/null || true
            kill -9 "$pid" 2>/dev/null || true
        done < "$TEST_PIDS_FILE"
        # rm -f "$TEST_PIDS_FILE"
    fi
    rm -rf "$TEST_TEMP_DIR"
}

setup() {
    # Create unique temporary directory for each test
    # export TEST_CASE_DIR="$TEST_TEMP_DIR/case_${BATS_TEST_NUMBER}"
    export TEST_CASE_DIR="$TEST_TEMP_DIR/case_${BATS_TEST_NUMBER}_${RANDOM}"
    mkdir -p "$TEST_CASE_DIR"
    cd "$TEST_CASE_DIR"
}

teardown() {
	# Cleanup test-specific processes
    if [[ -f "$TEST_PIDS_FILE" ]]; then
        while read -r pid; do
            pkill -P "$pid" 2>/dev/null || true
            kill -9 "$pid" 2>/dev/null || true
        done < "$TEST_PIDS_FILE"
        : > "$TEST_PIDS_FILE"
    fi
    cd "$PROJECT_ROOT"
    rm -rf "$TEST_CASE_DIR"
}

run_with_timeout() {
    local timeout=$1
    shift

    # Create a new process group
    set -m
    "$@" &
    local pid=$!
    echo $pid >> "$TEST_PIDS_FILE"

    # Wait for command with timeout
    local count=0
    while kill -0 $pid 2>/dev/null; do
        if ((count >= timeout)); then
            # Kill the entire process group
            pkill -P "$pid" 2>/dev/null || true
            kill -9 -"$pid" 2>/dev/null || true
            echo "Command timed out after $timeout seconds: $*" >&2
            return 124  # Return timeout status
        fi
        sleep 1
        ((count++))
    done

    wait $pid
    return $?
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
    valgrind --leak-check=full \
        -s \
        --show-reachable=yes \
        --errors-for-leak-kinds=all \
        --error-exitcode=33 \
        --track-origins=yes \
        --track-fds=yes \
        --suppressions="$PROJECT_ROOT/mini.supp" \
        "$MINISHELL_PATH" <<< "$@"
    # cd "$TEST_CASE_DIR"
}

assert_minishell_equal_bash() {
    run bash_execute "$@"
    local bash_status=$status
	local bash_output=$(echo "$output" | sed 's/bash: /minishell: /g')

    run minishell_execute "$@"

    if ! [[ $bash_output == $output ]]; then
		# local bash_file="./WIP_test/bash_$BATS_TEST_NAME.txt"
		#       local mini_file="./WIP_test/mini_$BATS_TEST_NAME.txt"
        echo "$bash_output" > BASH_OUTPUT_FILE
        echo "$output" > MINI_OUTPUT_FILE
		echo -e "===> bash_output:\n<$bash_output>\n\n===> minishell_output:\n<$output>"
        # echo -e $bash_output
        # echo -e $output
        echo -e "\ndiff:\n====================\n"
        diff BASH_OUTPUT_FILE MINI_OUTPUT_FILE || true
        echo -e "\n====================\n"
        # echo -e "===> bash_output:\n<$bash_output>\n===> minishell_output:\n<$output>"
        # echo -e "\ndiff:\n====================\n"
        # diff "$BASH_OUTPUT_FILE" "$MINI_OUTPUT_FILE" || true
        # echo -e "\n====================\n"
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
		echo -e "===> bash_out_norm:\n<$bash_output_heredoc>\n===> minishell_output:\n<$output>"
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
