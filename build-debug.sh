#!/bin/bash

set -e  # Exit on any error

# Ensure vcpkg is bootstrapped
if [ ! -f "./vcpkg/vcpkg" ]; then
    echo "Bootstrapping vcpkg..."
    ./vcpkg/bootstrap-vcpkg.sh
fi

# Set environment variables with absolute paths
export GEN=ninja
export VCPKG_TOOLCHAIN_PATH="$(pwd)/vcpkg/scripts/buildsystems/vcpkg.cmake"

echo "Building with vcpkg toolchain: $VCPKG_TOOLCHAIN_PATH"
echo "Using cmake: $(which cmake)"
echo "Using ninja: $(which ninja)"

# Clean and build
make debug

# Build the project
cmake --build build/debug --config Debug
