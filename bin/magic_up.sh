#!/usr/bin/env bash

# Change directory to two levels above the script's location
cd "$(dirname "$0")/../../" || { echo "Failed to navigate two levels up."; exit 1; }

# Run the passed commands
"$@"
