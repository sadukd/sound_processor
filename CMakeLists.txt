cmake_minimum_required(VERSION 3.14)

project(sound_processor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ─────────────────────────────
# LIBRARIES
# ─────────────────────────────

add_library(parser
    src/parser.cpp
)

add_library(waveform
    src/waveform.cpp
)

add_library(wav_reader
    src/wav_reader.cpp
)

target_include_directories(parser PUBLIC include)
target_include_directories(waveform PUBLIC include)
target_include_directories(wav_reader PUBLIC include)

# ─────────────────────────────
# APP
# ─────────────────────────────

add_executable(app
    src/main.cpp
)

target_link_libraries(app
    parser
    waveform
    wav_reader
)

# ─────────────────────────────
# CATCH2
# ─────────────────────────────

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.0
)

FetchContent_MakeAvailable(Catch2)

include(CTest)
include(Catch)

# ─────────────────────────────
# PARSER TESTS
# ─────────────────────────────

add_executable(parser_tests
    tests/test_parser.cpp
)

target_link_libraries(parser_tests
    parser
    Catch2::Catch2WithMain
)

catch_discover_tests(parser_tests)

# ─────────────────────────────
# WAVEFORM TESTS
# ─────────────────────────────

add_executable(waveform_tests
    tests/test_waveform.cpp
)

target_link_libraries(waveform_tests
    waveform
    Catch2::Catch2WithMain
)

catch_discover_tests(waveform_tests)

# ─────────────────────────────
# WAV READER TESTS (НОВОЕ)
# ─────────────────────────────

add_executable(wav_reader_tests
    tests/test_wav_reader.cpp
)

target_link_libraries(wav_reader_tests
    wav_reader
    waveform
    Catch2::Catch2WithMain
)

catch_discover_tests(wav_reader_tests)