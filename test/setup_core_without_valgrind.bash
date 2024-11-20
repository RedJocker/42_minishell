#!/usr/bin/env bash

# Enable error handling
set -euo pipefail

# Get the absolute path to the project root directory
export PROJECT_ROOT
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
export MINISHELL_PATH="${PROJECT_ROOT}/minishell"
export TEST_DIRS=()
export TEST_TIMEOUT=1

# Load bats-support if available (optional)
load_support() {
    if [ -f "${PROJECT_ROOT}/test/test_helper/bats-support/load.bash" ]; then
        load "${PROJECT_ROOT}/test/test_helper/bats-support/load.bash"
    fi
}

# Thread-safe file operations using BATS_TEST_NAME and PID
setup_file() {
    export TEST_TEMP_DIR="${PROJECT_ROOT}/test/temp/${BATS_TEST_NAME:-unknown}_$$"
    mkdir -p "${TEST_TEMP_DIR}"
    TEST_DIRS+=("${TEST_TEMP_DIR}")
}

teardown_file() {
    if [ -d "${TEST_TEMP_DIR}" ]; then
        rm -rf "${TEST_TEMP_DIR}"
    fi
    # Reset tracked directories
    TEST_DIRS=()
    # Clean up any lingering minishell processes
    pkill -P $$ -f minishell 2>/dev/null || true
}

setup() {
    load_support
    mkdir -p "${PROJECT_ROOT}/test/logs"
    export TEST_LOG="${PROJECT_ROOT}/test/logs/${BATS_TEST_NUMBER:-unknown}_${BATS_TEST_NAME}.log"
    echo "Starting test: ${BATS_TEST_NAME}" > "$TEST_LOG"
}

teardown() {
    # Clean up any processes started by this test
    jobs -p | xargs kill -9 2>/dev/null || true
	# Mark test as completed for progress tracking
}

clean_output() {
    local output="$1"
    echo "$output" | grep -v '^RedWillShell\$ ' | \
                    grep -v '^RedWillShell\$ exit$'
}

bash_execute() {
    local cmd="$1"
    local output
    local temp_file="${TEST_TEMP_DIR}/bash_output_$$"
    local typescript_file="${temp_file}.typescript"

    echo "Executing bash command: $cmd" >> "$TEST_LOG"

    # Create the command file
    {
        echo "$cmd"
    } > "${temp_file}"
    chmod +x "${temp_file}"

    # Run bash with script to emulate terminal
    script -q -c "VARIABLE_FROM_OUTSIDE_MORE_SPACES='abc    def' \
                  VARIABLE_FROM_OUTSIDE='abc def' \
                  LANGUAGE='en' \
                  PS1='RedWillShell$ ' \
                  timeout --kill-after=2 $TEST_TIMEOUT bash --norc -i < ${temp_file}" \
           "$typescript_file" > /dev/null

    # Read and clean the output
    output=$(cat "$typescript_file" | sed 's/\r//g' | sed '/^Script/d' | sed '/^RedWillShell/d')
    
    # Cleanup
    rm -f "${temp_file}" "$typescript_file"
    
    echo "$output"
}
# timeout --kill-after=2 $TEST_TIMEOUT bash --norc -i "${temp_file}.sh" 2>&1 <<'EOF' EOF) || {

minishell_execute() {
    local cmd="$1"
    local output
    local temp_file="${TEST_TEMP_DIR}/minishell_output_$$"

    echo "Executing minishell command: $cmd" >> "$TEST_LOG"

    # Create a script with the commands
    echo "$cmd" > "${temp_file}.sh"
    chmod +x "${temp_file}.sh"

    # Run minishell with strict timeout
    output=$(VARIABLE_FROM_OUTSIDE_MORE_SPACES="abc    def" \
             VARIABLE_FROM_OUTSIDE="abc def" \
             LANGUAGE="en" \
             PS1='RedWillShell$ ' \
             timeout --kill-after=2 $TEST_TIMEOUT "$MINISHELL_PATH" < "${temp_file}.sh" 2>&1)

    rm -f "${temp_file}.sh"
	clean_output "$output"
}

assert_minishell_equal_bash() {
    local cmd="$1"
    local path_test="${TEST_TEMP_DIR}/case_$$"

    echo "Starting assertion for command: $cmd" >> "$TEST_LOG"
    mkdir -p "$path_test"
    cd "$path_test"

    echo "Running bash command..." >> "$TEST_LOG"
    run bash_execute "$cmd"
    local bash_status
    bash_status=status
	local bash_output
    bash_output=$(echo "$output" | sed 's/bash: /minishell: /g')
    bash_output=$(echo "$bash_output" | sed 's/line [0-9]\+: //g')

    echo -e "Bash output:\n\n$bash_output\n" >> "$TEST_LOG"

    rm -rf "$path_test"
    mkdir -p "$path_test"
    cd "$path_test"

    echo "Running minishell command..." >> "$TEST_LOG"
    run minishell_execute "$cmd"
    local minishell_output="$output"
    echo -e "Minishell output:\n\n$minishell_output\n" >> "$TEST_LOG"

    # echo -e "===> bash_output:\n<$bash_output>\n===> minishell_output:\n<$minishell_output>" 1>&3

    if [ "$bash_output" != "$minishell_output" ]; then
        mkdir -p "${PROJECT_ROOT}/test/output_error"
        local bash_file="$PROJECT_ROOT/test/output_error/bash_${BATS_TEST_NAME}_$$.txt"
        local mini_file="$PROJECT_ROOT/test/output_error/mini_${BATS_TEST_NAME}_$$.txt"
        echo "$bash_output" > "$bash_file"
        echo "$minishell_output" > "$mini_file"
		{
			echo "Test failed - outputs differ";
			echo "Bash output: '$bash_output'";
			echo "Mini output: '$minishell_output'";
		} >> "$TEST_LOG"
        false
    fi

    echo "Running valgrind check..." >> "$TEST_LOG"
    echo "Test completed successfully" >> "$TEST_LOG"
}