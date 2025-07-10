#!/usr/bin/env bash

BASE_DIR=$(readlink -f "$(dirname "$0")/..")

DIRS=(
    "$BASE_DIR/apps"
    "$BASE_DIR/install"
    "$BASE_DIR/edge_list"
    "$BASE_DIR/graph_file"
)

for dir in "${DIRS[@]}"; do
    if [ ! -d "$dir" ]; then
        mkdir -p "$dir"
        echo "Created: $dir"
    else
        echo "Already exists: $dir"
    fi
done
