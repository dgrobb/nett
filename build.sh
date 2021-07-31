#!/usr/bin/env bash
#
# Sets up the required environment to build the checker
# and runs the build operations. The resulting binary
# will be placed under llvm-project/build/bin.

SCRIPT_PATH="$( cd "$(dirname "$0")" > /dev/null 2>&1; pwd -P )"

echo "[+] Starting build"

# Check to see if ninja and cmake have been installed or not
# If they do not exist, then stop the installation immediately
if ! command -v cmake &> /dev/null 
then
    echo "[-] 'cmake' could not be found. Aborting build"
    exit 1
fi

if ! command -v ninja &> /dev/null 
then
    echo "[-] 'ninja' could not be found. Aborting build"
    exit 1
fi

# Change to the llvm-project directory
cd "${SCRIPT_PATH}/../../"

# Add the appropriate entry into the CMakeLists.txt file
# if it hasn't been added already
grep nett clang-tools-extra/CMakeLists.txt > /dev/null 2> /dev/null

if [ $? -ne 0 ]; then
    echo "[+] Adding 'nett' entry into clang-tools-extra/CMakeLists.txt"
    echo -e "\nadd_subdirectory(nett)" >> clang-tools-extra/CMakeLists.txt
fi

# Make the build directory if it does not already exist
if [ ! -d build ]; then
    echo "[+] Creating 'build' directory under 'llvm-project'"
    mkdir -p build

    if [ $? -ne 0 ]; then
        echo "[-] Unable to create 'build' directory. Aborting build"
        exit 1
    fi
fi

# Move into the build directory
cd build

# Run the cmake initialisation
echo "[+] Running cmake initialisation sequence"
cmake -G Ninja ../llvm \
    -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" \
    -DCMAKE_BUILD_TYPE:STRING=Release

if [ $? -ne 0 ]; then
    echo "[-] Something went wrong when running cmake. Exiting build"
    exit 1
fi

# Build the checker
echo "[+] Building executable"
ninja nett

if [ $? -ne 0 ]; then
    echo "[-] Something went wrong when building the executable. Exiting build"
    exit 1;
fi

echo "[+] Build complete. The 'nett' executable can be found under 'llvm-project/build/bin'"