#!/bin/bash

export OMP_NUM_THREADS=10

# Finds the path where this script is.
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

# Goes there
cd $SCRIPT_DIR

# Adds to python path
export PYTHONPATH="${PYTHONPATH}:$(pwd)"
export OBJC_DISABLE_INITIALIZE_FORK_SAFETY=YES