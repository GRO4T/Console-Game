#!/bin/bash
# TODO: Detect operating system
bazel build --config="$1" -c "$2" //:ascii-combat