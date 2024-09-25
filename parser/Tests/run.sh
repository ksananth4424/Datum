#!/usr/bin/env bash
shopt -s lastpipe 
EXEC=../parser.o
passed=0
failed=0
correct_files=0
wrong_files=0

run_test() {
    local file=$1
    local test_type=$2
    local expected_error=$3

    # Execute the parser and log output
    ${EXEC} ${file} > log 2>&1

    # Count errors and warnings
    actual_error=$(grep -c error log)

    if [[ ($actual_error -eq $expected_error)]]; then
        echo -e "\e[32m\t [ PASSED ] \e[0m $file"
        passed=$((passed+1))
    else
        echo -e "\e[31m\t [ FAILED ] \e[0m $file \tfound errors: ${actual_error}/${expected_error}"
        cat log
        echo -e "\n"
        failed=$((failed+1))
    fi
    rm log
}

if [[ ! -f ${EXEC} ]]; then
    echo "Executable does not exist"
    exit 2
fi

# test correct syntax files
echo -e "\e[1mTesting correct files\e[0m"
echo "---------------------"

for file in $(ls *.dt | grep correct); 
do
    correct_files=$((correct_files + 1))
    run_test $file "correct" 0 0
done

echo -e "\n"

# test wrong syntax files
echo -e "\e[1mTesting incorrect files\e[0m"
echo "---------------------"

for file in $(ls *.dt | grep wrong); 
do
    wrong_files=$((wrong_files + 1))
    expected_error=$(grep -c ERROR_EXPECTED $file)
    expected_warning=$(grep -c WARNING_EXPECTED $file)
    run_test $file "incorrect" $expected_error $expected_warning
done

echo -e "\n"

# test stats
echo -e "\tTest summary"
echo -e "Total \t:\t$((passed + failed)) (Correct count: $correct_files, Wrong count: $wrong_files)"
echo -e "Passed \t:\t$passed"
echo -e "Failed \t:\t$failed"

# exit
if [[ $failed -ne 0 ]]; then
    exit 1
fi
