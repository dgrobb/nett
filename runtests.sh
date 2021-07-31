#!/usr/bin/env bash
#
# Runs the unit tests on the built 'nett' binary to verify that
# it works correctly. 

SCRIPT_PATH="$( cd "$(dirname "$0")" > /dev/null 2>&1; pwd -P )"

NETT_PATH="${SCRIPT_PATH}/../../build/bin/nett"

# Check if 
# Check to see if the binary has been built
if [ ! -f "${NETT_PATH}" ]; then 
    echo "No nett binary found under llvm-project/build/bin"
    echo "You can build the binary by running the build.sh script"
    exit 1
fi

# If it has been built, run the tests
python3 "${SCRIPT_PATH}/test/runner.py"