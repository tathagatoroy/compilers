# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.14)

# Set CMake policies to avoid warnings
cmake_policy(SET CMP0135 NEW)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Set the project name and C++ standard
project(Kaleidoscope LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Fetch and configure Google Test ---
# This automatically downloads and prepares Google Test for our project.
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For projects with sensitive URLs, options such as `GIT_REPOSITORY` and `GIT_TAG` may be used instead.
FetchContent_MakeAvailable(googletest)

# --- Configure Project Structure and Files ---

# Create a library for our lexer. This allows it to be shared
# between the main compiler and the test executable.
add_library(LexerLib src/lexer.cpp)

# Add the 'src' directory as a public include directory for the library.
# This allows other targets that link to LexerLib to find its headers.
target_include_directories(LexerLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)

# --- Configure Testing ---

# Enable testing for this project
enable_testing()

# Create the test executable
add_executable(run_lexer_tests tests/test_lexer.cpp)

# Link the test executable against our Lexer library and Google Test
target_link_libraries(run_lexer_tests PRIVATE LexerLib gtest_main)

# Include the Google Test header directory for the test executable
target_include_directories(run_lexer_tests PRIVATE ${googletest_SOURCE_DIR}/googletest/include)

# Add the test to CTest (CMake's testing framework)
include(GoogleTest)
gtest_discover_tests(run_lexer_tests)

# --- Configure Output Directories ---

# Set the output directory for the test executable
set_target_properties(run_lexer_tests
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/testExecutables"
)
