# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    parallel_helper.bash                               :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/26 03:15:43 by dande-je          #+#    #+#              #
#    Updated: 2024/10/26 03:58:50 by dande-je         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# This ensures thread-safe file operations
setup_file() {
    # Use PID in temporary files to avoid conflicts
    export TEST_TEMP_DIR="./test/temp_$$"
    mkdir -p "$TEST_TEMP_DIR"
    
    # Use unique files for each test process
    export BASH_OUTPUT_FILE="$TEST_TEMP_DIR/bash_${BATS_TEST_NAME}_${PPID}.txt"
    export MINI_OUTPUT_FILE="$TEST_TEMP_DIR/mini_${BATS_TEST_NAME}_${PPID}.txt"
}

teardown_file() {
    rm -rf "$TEST_TEMP_DIR"
}

setup() {
    # Create unique temporary directory for each test
    export TEST_CASE_DIR="$TEST_TEMP_DIR/case_${BATS_TEST_NUMBER}"
    mkdir -p "$TEST_CASE_DIR"
}

teardown() {
    rm -rf "$TEST_CASE_DIR"
}

bash_execute() {
    cd "$TEST_CASE_DIR"
    VARIABLE_FROM_OUTSIDE_MORE_SPACES="abc    def" \
    VARIABLE_FROM_OUTSIDE="abc def" \
    LANGUAGE="en" \
    PS1='RedWillShell$ ' \
    bash --norc -i <<< "$@"
    cd - > /dev/null
}

minishell_execute() {
    cd "$TEST_CASE_DIR"
    VARIABLE_FROM_OUTSIDE_MORE_SPACES="abc    def" \
    VARIABLE_FROM_OUTSIDE="abc def" \
    LANGUAGE="en" \
    PS1='RedWillShell$ ' \
    ../../../minishell <<< "$@"
    cd - > /dev/null
}

assert_minishell_equal_bash() {
    run bash_execute "$@"
    local bash_status=$status
    local bash_output=$output
    
    run minishell_execute "$@"

    if ! [[ $bash_output == $output ]]; then
        echo "$bash_output" > "$BASH_OUTPUT_FILE"
        echo "$output" > "$MINI_OUTPUT_FILE"
        echo -e "===> bash_output:\n<$bash_output>\n===> minishell_output:\n<$output>"
        echo -e "\ndiff:\n====================\n"
        diff "$BASH_OUTPUT_FILE" "$MINI_OUTPUT_FILE" || true
        echo -e "\n====================\n"
        false
    fi

    if ! [[ $bash_status == $status ]]; then
        echo -e "===> bash_status: $bash_status\nminishell_status: $status"
        false
    fi
}
