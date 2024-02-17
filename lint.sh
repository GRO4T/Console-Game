#!/bin/bash
# TODO(GRO4T): integrate linting directly into Bazel

echo "============================== cpplint (headers) ============================="
cpplint include/*.h

echo "============================== cppcheck (headers) ============================="
cppcheck --language=c++ --std=c++20 include/*.h

echo "============================== clang-tidy (headers) ==========================="
clang-tidy include/*.h -- -std=c++20 -x c++ -Wno-pragma-once-outside-header 

echo "============================== cpplint (sources) =============================="
cpplint src/*.cc

echo "============================== cppcheck (sources) ============================="
cppcheck --language=c++ --std=c++20 src/*.cc

echo "============================== clang-tidy (sources) ==========================="
clang-tidy src/*.cc -- -Iinclude/ -Ibazel-bin/sfml/include -std=c++20 -Wall -Wextra

echo "================================= Finished ==================================="