#!/bin/sh

# Clean-up any previous CMake invocation
rm -rf CMakeCache.txt

# Set-up the environment
export CC=/usr/bin/clang
export CXX=/usr/bin/clang++ 

# Re-run CMake
mkdir -p build
cd build
cmake ../CMakeLists.txt
