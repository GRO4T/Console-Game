#!/bin/bash
# TODO: Detect operating system
set -o errexit

target="$3"
if [[ -z $target ]]; then
    target="all"
fi

debug_flags=""
if [[ -n $DEBUG ]]; then
    debug_flags="-s --verbose_failures --sandbox_debug"
fi

if [[ "$2" == debug ]]; then
    bazel build $debug_flags --noenable_bzlmod --config="$1" -c dbg "//:$target"
elif [[ "$2" == release ]]; then
    # NOTE: Compilation with -c opt results with signal 11 from linker.
    bazel build $debug_flags --noenable_bzlmod --config="$1" -c fastbuild --copt="-O2" "//:$target"
fi
