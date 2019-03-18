cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

project(ConsoleGame)

ADD_LIBRARY(Module
    main.cpp
    Creature.cpp
    )
target_link_libraries(Module -lcurses)

add_executable(
    ConsoleGame
    main.cpp
    Creature.cpp 
    )

target_link_libraries(ConsoleGame Module)
