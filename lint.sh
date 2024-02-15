#!/bin/bash
echo "============================== cpplint (headers) ============================="
cpplint include/*.h

echo "============================== cppcheck (headers) ============================="
cppcheck --language=c++ --std=c++20 include/*.h

echo "============================== clang-tidy (headers) ==========================="
clang-tidy include/*.h -- -std=c++20 -x c++ -Wno-pragma-once-outside-header 

echo "============================== cpplint (sources) =============================="
cpplint src/*.cc

echo "============================== cppcheck (sources) ============================="
cppcheck --language=c++ --std=c++11 src/*.cc

echo "============================== clang-tidy (sources) ==========================="
clang-tidy src/*.cc -- -Iinclude/ -std=c++20 -Wall -Wextra

echo "================================= Finished ==================================="