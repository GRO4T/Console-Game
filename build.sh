#!/bin/bash
# TODO: Detect operating system
set -o errexit
bazel build --jobs 8 --noenable_bzlmod --config="$1" -c "$2" //:all