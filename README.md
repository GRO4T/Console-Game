How to create Debug and Release mode in cmake
---------------------------------------------
---------------------------------------------
    create directiories "Debug" and "Release"
    -----------------------------------------
    
    in each of directory run:
        -in Debug
            cmake -DCMAKE_BUILD_TYPE=Debug ..
        -in Release
            cmake -DCMAKE_BUILD_TYPE=Release ..
