include(FetchContent)
FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG        v2.7.2
)
FetchContent_MakeAvailable(CLI11)

add_library(microgen_lib BooleanModel.cpp ${PROJECT_SOURCE_DIR}/include/BooleanModel.hpp)
target_include_directories(microgen_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(microgen main.cpp)
target_link_libraries(microgen PRIVATE microgen_lib CLI11::CLI11)