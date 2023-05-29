cmake_minimum_required(VERSION 3.25)
project(Eksamen2023)

set(CMAKE_CXX_STANDARD 23)

add_executable(Eksamen2023 main.cpp)
add_executable(Requirement5_1 Requirement5_1.cpp Simulator/Reagent.h Simulator/Helper.h Simulator/STable.h Simulator/reaction.cpp Simulator/reaction.h Simulator/Simulator.cpp Simulator/Simulator.h)
add_executable(Requirement5_2 Requirement5_2.cpp Simulator/Reagent.h Simulator/Helper.h Simulator/STable.h Simulator/reaction.cpp Simulator/reaction.h Simulator/Simulator.cpp Simulator/Simulator.h)
add_executable(Requirement5_3 Requirement5_3.cpp Simulator/Reagent.h Simulator/Helper.h Simulator/STable.h Simulator/reaction.cpp Simulator/reaction.h Simulator/Simulator.cpp Simulator/Simulator.h Benchmarking/timer.h Benchmarking/timer.cpp Benchmarking/benchmarking.h Benchmarking/benchmarking.cpp)
add_executable(STableTests STableTests.cpp Simulator/STable.h doctest.h)