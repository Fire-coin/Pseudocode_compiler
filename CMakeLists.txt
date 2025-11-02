cmake_minimum_required(VERSION 3.20)
project(PseudocodeTranspiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)

include(FetchContent)

# FetchContent_Declare(
#   abseil
#   GIT_REPOSITORY https://github.com/abseil/abseil-cpp/archive/refs/tags/20250127.0.zip
# )
# FetchContent_MakeAvailable(abseil)

# --- Abseil ---
set(ABSL_ENABLE_INSTALL ON)
set(ABSL_BUILD_TESTING OFF)
set(ABSL_PROPAGATE_CXX_STD ON)
FetchContent_Declare(
  abseil
  GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
  GIT_TAG 20250814.1
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(abseil)

# --- RE2 ---
FetchContent_Declare(
  re2
  GIT_REPOSITORY https://github.com/google/re2.git
  GIT_TAG 2024-07-02
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(re2)

# --- Your App ---
add_executable(PseudocodeTranspiler
    src/main.cpp
    src/lexer.cpp
    src/string_utils.cpp
)

target_include_directories(PseudocodeTranspiler PRIVATE include)
target_link_libraries(PseudocodeTranspiler PRIVATE re2)
