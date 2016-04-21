#!/bin/bash

# Usage: ./capture.sh /dev/ttyACM0

while IFS='' read -r line || [[ -n "$line" ]]; do
    echo "Output: $line"
done < "$1"