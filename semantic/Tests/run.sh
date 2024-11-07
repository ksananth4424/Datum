#!/usr/bin/env bash
shopt -s lastpipe 
EXEC=../semantic.o
CORR_TESTFILES=`ls *.dt | grep correct`
passed=0
failed=0
if [ -f ${EXEC} ]
then  
    echo -e "\e[1mTesting correct files\e[0m"
    echo "---------------------"

    for file in ${CORR_TESTFILES}
    do
        ./${EXEC} ${file} > log  2>&1
        error=`grep "Error:" log -c`
        if [[ ($error -eq 0) ]]
        then
            echo -e "\e[32m\t [ PASSED ] \e[0m $file\n"
            passed=$(($passed+1));
        else
            echo -e "\e[31m\t [ FAILED ] \e[0m $file \t error: ${error}\n"
            cat log 
            failed=$(($failed+1))
        fi
        rm log
    done
else
    echo "Executable does not exist"
    exit 2
fi

# Checking for Error code

echo -e "\e[1mTesting incorrect files\e[0m"
echo "---------------------"

ERROR_FILES=`ls *.dt | grep error`
for file in $ERROR_FILES
do
    ./${EXEC} ${file} > log  2>&1
    expected_error=`grep "ERROR_EXPECTED" $file -c`
    error=`grep "Error:" log -c`
    if [[ ($error -eq $expected_error) ]]
    then
        echo -e "\e[32m\t [ PASSED ] \e[0m $file \tfound errors: ${error}/${expected_error}\n"
        passed=$(($passed+1))
    else
        echo -e "\e[31m\t [ FAILED ] \e[0m $file \tfound errors: ${error}/${expected_error}\n"
        failed=$(($failed+1))
    fi
    rm log
done

echo -e "\tTest summary"
echo -e "Total \t:\t$(($passed + $failed))"
echo -e "Passed \t:\t$passed"
echo -e "Failed \t:\t$failed"

if [ $failed -ne 0 ]
then
    exit 1
fi