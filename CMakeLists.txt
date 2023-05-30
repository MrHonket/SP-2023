cmake_minimum_required(VERSION 3.25)

project(Eksamen2023)

set(CMAKE_CXX_STANDARD 23)

add_executable(Eksamen2023 main.cpp Simulator/Reagent.h Simulator/Helper.h Simulator/STable.h Simulator/reaction.cpp Simulator/reaction.h Simulator/Simulator.cpp Simulator/Simulator.h Benchmarking/timer.h Benchmarking/timer.cpp Benchmarking/benchmarking.h Benchmarking/benchmarking.cpp)
add_executable(STableTests STableTests.cpp Simulator/STable.h doctest.h)