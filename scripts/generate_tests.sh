#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "${SCRIPT_DIR}")"
GEN_DIR="${PROJECT_ROOT}/build/tests"

if [ $# -ne 4 ]; then
    echo "Usage: bash $0 <tests_number> <keys number> <min_value> <max_value>"
    exit 1
fi

TESTS_NUM=$1;
KEYS_NUM=$2;
MIN_VAL=$3;
MAX_VAL=$4;

echo -e "${TESTS_NUM}\n${KEYS_NUM}\n${MIN_VAL}\n${MAX_VAL}" | "${GEN_DIR}/generator" "${PROJECT_ROOT}"