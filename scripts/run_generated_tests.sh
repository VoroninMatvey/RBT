#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "${SCRIPT_DIR}")"

tests_directory="${PROJECT_ROOT}/data/tests/tests/"
answers_directory="${PROJECT_ROOT}/data/tests/answers/"
range_query="${PROJECT_ROOT}/build/range_query"
border=$(find ${tests_directory} -type f | wc -l)

failed_tests=""
passed_count=0

echo ""
echo -e "\e[35;1mGenerated tests\e[0m"
for i in $(seq 1 1 ${border})
do  
    echo -e "\e[34mTest${i}\e[0m"
    if diff -w "${answers_directory}answer${i}.txt" <("${range_query}" < "${tests_directory}test${i}.txt") > /dev/null; then
        echo -e "\e[36mwork answer:    \e[32m[OK]\e[0m"
        ((passed_count++))
    else
        echo -e "\e[36mwork answer:    \e[31m[FAIL]\e[0m"
        failed_tests="${failed_tests} ${i}"
    fi
    echo ""
done

echo "----------------------------------------"
echo -e "\e[35;1mTESTING SUMMARY\e[0m"
echo -e "Total tests: ${border}"
echo -e "Passed:      \e[32m${passed_count}\e[0m"

if [ -z "$failed_tests" ]; then
    echo -e "Failed:      \e[32mNone"
else
    failed_count=$((border - passed_count))
    echo -e "Failed:      \e[31m${failed_count}\e[0m (Tests numbers:\e[31m${failed_tests}\e[0m)"
fi
echo "----------------------------------------"