
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/b66cffcd5f5d6ee34a4f96d57bf016d9a22cb023.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(test_boolean_model test_boolean_model.cpp)
target_link_libraries(test_boolean_model PRIVATE microgen_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test_boolean_model)