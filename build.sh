#!/bin/bash

set -x

g++ -std=c++17 -Wall -Wpedantic COO_class.cpp CSR_class.cpp main.cpp -I. -o sparse_matrix

set +x

if [ $? -eq 0 ]; then
    echo "Build successful! You can run the program using ./sparse_matrix"
else
    echo "Build failed."
fi