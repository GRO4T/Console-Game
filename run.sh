#!/bin/bash
set -o errexit
./build.sh "$@"
./bazel-bin/ascii-combat