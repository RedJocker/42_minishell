#!/bin/bash
# Clean old test results
rm -rf test/output_error > /dev/null || true

# Colors and symbols
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color
PROGRESS_BAR_WIDTH=50

# Function to get total number of tests
get_total_tests() {
    local total
    total=$(find test -name "*.bats" -exec cat {} \; | grep -c "^@test")
    echo "$total"
}

# Function to draw progress bar
draw_progress_bar() {
    local current=$1
    local total=$2
    local percentage=$((current * 100 / total))
    if [ $percentage -gt 100 ]; then
        percentage=100
    fi
    local filled=$((percentage * PROGRESS_BAR_WIDTH / 100))
    local empty=$((PROGRESS_BAR_WIDTH - filled))
    
    printf "\r["
    printf "%${filled}s" | tr ' ' '#'
    printf "%${empty}s" | tr ' ' '-'
    printf "] %3d%% (%d/%d tests)" "${percentage}" "${current}" "${total}"
}

# Function to run tests with progress
run_tests_with_progress() {
    local total_tests
    total_tests=$(get_total_tests)
    local current_test=0
    local failed_tests=0
    local test_output_file
    test_output_file=$(mktemp)
    
    echo -e "${BLUE}Running ${total_tests} tests...${NC}\n"
    
    # Run bats and process output in real-time
    bats --jobs 4 test/*/*.bats 2>&1 | {
        while IFS= read -r line; do
            echo "$line" >> "$test_output_file"
            if [[ $line =~ ^[[:space:]]*[0-9]+[[:space:]]*test ]]; then
                ((current_test++))
                if [ $current_test -le "${total_tests}" ]; then
                    draw_progress_bar "$current_test" "$total_tests"
                fi
            fi
            if [[ $line == *"failure"* ]]; then
                ((failed_tests++))
            fi
        done
        
        # Ensure we show 100% at the end
        draw_progress_bar "$total_tests" "$total_tests"
        
        echo -e "\n\n${BLUE}Test Results:${NC}"
        
        if [ $failed_tests -eq 0 ]; then
            echo -e "${GREEN}All tests passed successfully!${NC}"
        else
            echo -e "${RED}${failed_tests} tests failed.${NC}"
            echo -e "\nDetailed test output:"
            cat "$test_output_file"
        fi
        
        rm -f "$test_output_file"
    }
}

# Disable input buffering
stty -icanon min 0 time 0

# Main execution
echo "Starting test suite..."
run_tests_with_progress

# Reset terminal settings
stty sane
