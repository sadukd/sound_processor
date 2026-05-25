cmake_minimum_required(VERSION 3.14)
project(sound_processor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ─────────────────────────────
# LIBRARY (твой код)
# ─────────────────────────────
add_library(parser
    src/parser.cpp
)

target_include_directories(parser PUBLIC include)

# ─────────────────────────────
# APP (если есть main)
# ─────────────────────────────
add_executable(app
    src/main.cpp
)

target_link_libraries(app parser)

# ─────────────────────────────
# CATCH2 (автозагрузка)
# ─────────────────────────────
include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.0
)

FetchContent_MakeAvailable(Catch2)

# ─────────────────────────────
# TESTS
# ─────────────────────────────
add_executable(tests
    tests/test_parser.cpp
)

target_link_libraries(tests
    parser
    Catch2::Catch2WithMain
)

# ─────────────────────────────
# CTEST интеграция
# ─────────────────────────────
include(CTest)
include(Catch)

catch_discover_tests(tests)