#!/bin/bash
set -o errexit

echo "======================== Clang Format ==================================="
clang-format -i include/*.h src/*.cc

echo "======================== Copyright fix =================================="
add_copyright() {
    paths=("$@")
    copyright_text="\/\* Copyright 2024 Damian Kolaska \*\/"
    for file in "${paths[@]}"; do
        if [ -f "$file" ] && [ -r "$file" ]; then
            if ! grep -q "^${copyright_text}$" "$file"; then
                echo "Adding copyright clause to $file"
                sed -i '' "1s/^/${copyright_text}\n/" "$file"
            fi
        fi
    done
}

add_copyright ./include/*.h
add_copyright ./src/*.cc

echo "======================== Clang Tidy fix ================================="
clang-tidy -fix-errors src/*.cc -- -Iinclude/ -std=c++20


echo "======================== Finished ======================================="