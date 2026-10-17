cmake_minimum_required(VERSION 3.31)

project(microgen VERSION 0.0.1
                 DESCRIPTION "microgen"
                 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG        v2.7.2
)
FetchContent_MakeAvailable(CLI11)

add_library(microgen_lib src/BooleanModel.cpp include/microgen/BooleanModel.hpp)
add_executable(microgen src/main.cpp)

target_link_libraries(microgen PRIVATE microgen_lib CLI11::CLI11)