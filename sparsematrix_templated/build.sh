#!/bin/bash

set -x

g++ -std=c++17 -Wall -Wpedantic main.cpp -I. -o sparse_matrix_templated

if [ $? -eq 0 ]; then
    echo "Build successful! You can run the program using ./sparse_matrix_templated"
else
    echo "Build failed."
fi

set +x