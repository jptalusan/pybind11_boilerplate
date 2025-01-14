#!/bin/bash

export OMP_NUM_THREADS=10

# Finds the path where this script is.
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

# Goes there
cd $SCRIPT_DIR

# Adds to python path
export PYTHONPATH="$(pwd)"
# export PYTHONPATH="${PYTHONPATH}:$(pwd)"
export OBJC_DISABLE_INITIALIZE_FORK_SAFETY=YES
export TORCH_LIB_DIR="${SCRIPT_DIR}/extlib/libtorch"
export EIGEN3_INCLUDE_DIR="${SCRIPT_DIR}/extlib/eigen-3.4.0"