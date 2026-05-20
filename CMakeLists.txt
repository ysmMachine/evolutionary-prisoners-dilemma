cmake_minimum_required(VERSION 3.16)
project(EvolutionaryPrisonersDilemma VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(evo_pd_core
    src/PayoffMatrix.cpp
    src/Strategy.cpp
    src/Player.cpp
    src/Game.cpp
    src/Population.cpp
    src/Simulation.cpp
    src/CsvWriter.cpp
)

target_include_directories(evo_pd_core PUBLIC include)

add_executable(evo_pd src/main.cpp)
target_link_libraries(evo_pd PRIVATE evo_pd_core)

enable_testing()

add_executable(test_core tests/test_core.cpp)
target_link_libraries(test_core PRIVATE evo_pd_core)

add_test(NAME core_tests COMMAND test_core)
