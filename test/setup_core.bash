#!/usr/bin/env bash

# Get the absolute path to the project root directory
export PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
export MINISHELL_PATH="${PROJECT_ROOT}/minishell"
export TEST_DIRS=()

# This ensures thread-safe file operations
setup_file() {
	export TEST_TEMP_DIR="${PROJECT_ROOT}/test/temp/temp_${PPID}"
    mkdir -p "${TEST_TEMP_DIR}"
    TEST_DIRS+=("${TEST_TEMP_DIR}")
}

teardown_file() {
	# Cleanup all tracked directories
    for dir in "${TEST_DIRS[@]}"; do
        if [ -d "$dir" ]; then
            rm -rf "$dir"
        fi
    done
    # Reset tracked directories
    TEST_DIRS=()
}

setup() {
	true
}

teardown() {
	# Clean up test case directory
	if [ -d "${PROJECT_ROOT}/test/temp" ]; then
        rm -rf "${PROJECT_ROOT}/test/temp"
    fi
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
	local path_test="${PROJECT_ROOT}/test/temp/temp_${PPID}/case_${BATS_TEST_NAME}"
	mkdir -p "$path_test"
	TEST_DIRS+=("$path_test")
	cd "$path_test"

    run bash_execute "$@"
    local bash_status=$status
	local bash_output=$(echo "$output" | sed 's/bash: /minishell: /g')

    run minishell_execute "$@"

    echo -e "===> bash_output:\n<$bash_output>\n===> minishell_output:\n<$output>" 1>&3 

    if ! [[ "$bash_output" == "$output" ]]; then
		mkdir -p $PROJECT_ROOT/test/output_error
		local bash_file="$PROJECT_ROOT/test/output_error/bash_$BATS_TEST_NAME.txt"
		local mini_file="$PROJECT_ROOT/test/output_error/mini_$BATS_TEST_NAME.txt"
		echo "$bash_output" > "$bash_file"
		echo "$output" > "$mini_file"
		echo -e "===> bash_output:\n<$bash_output>\n\n===> minishell_output:\n<$output>"
        echo -e "\ndiff:\n====================\n"
        diff "$bash_file" "$mini_file" || true
        echo -e "\n====================\n"
        false
    fi

    run minishell_leak_check "$@"

    if (( $status == 33 )); then
		mkdir -p $PROJECT_ROOT/test/output_error
		local valgrind_file="$PROJECT_ROOT/test/output_error/valgrind_$BATS_TEST_NAME.txt"
		echo "$output" > "$valgrind_file"
		echo -e "VALGRIND ERROR:\n$output"
		false
    fi
}

assert_minishell_equal_bash_heredoc() {
	local path_test="${PROJECT_ROOT}/test/temp/temp_${PPID}/case_${BATS_TEST_NAME}"
	mkdir -p "$path_test"
	TEST_DIRS+=("$path_test")
	cd "$path_test"

    run bash_execute "$@"
    local bash_status=$status
    local bash_output=$output
    local bash_out_norm=$(awk 'NR > 2 && /here-document at line/ { gsub(/at line [0-9]+ /, "", $0); print $0} !/here-document/ { print $0}' <<< "$output")
	local bash_output_heredoc=$(echo "$bash_out_norm" | sed 's/bash: /minishell: /g')

    run minishell_execute "$@"

    if ! [[ "$bash_output_heredoc" == "$output" ]]; then
		mkdir -p $PROJECT_ROOT/test/output_error
		local bash_file="$PROJECT_ROOT/test/output_error/bash_$BATS_TEST_NAME.txt"
		local mini_file="$PROJECT_ROOT/test/output_error/mini_$BATS_TEST_NAME.txt"
        echo "$bash_output_heredoc" > "$bash_file"
        echo "$output" > "$mini_file"
		echo -e "===> bash_output:\n<$bash_output_heredoc>\n\n===> minishell_output:\n<$output>"
        echo -e "\ndiff:\n====================\n"
        diff "$bash_file" "$mini_file" || true
        echo -e "\n====================\n"
		false
    fi

    run minishell_leak_check "$@"

    if (( $status == 33 )); then
		mkdir -p $PROJECT_ROOT/test/output_error
		local valgrind_file="$PROJECT_ROOT/test/output_error/valgrind_$BATS_TEST_NAME.txt"
		echo "$output" > "$valgrind_file"
		echo -e "VALGRIND ERROR:\n$output\n"
		false
    fi
}
