include(FetchContent)
FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG        v2.7.2
)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
    URL_HASH SHA256=42f6e95cad6ec532fd372391373363b62a14af6d771056dbfc86160e6dfff7aa
)
    
FetchContent_MakeAvailable(CLI11 json)

add_library(microgen_lib BooleanModel.cpp ${PROJECT_SOURCE_DIR}/include/BooleanModel.hpp)
target_include_directories(microgen_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(microgen main.cpp)
target_link_libraries(microgen_lib PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(microgen PRIVATE microgen_lib CLI11::CLI11)