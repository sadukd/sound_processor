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

add_library(wav_writer
    src/wav_writer.cpp
)

add_library(filters
    src/filters/ampl.cpp
    src/filters/normalize.cpp
    src/filters/silence.cpp
    src/filters/timestretch.cpp
    src/filters/lowpass.cpp
    src/filters/generators.cpp
    src/filters/producers.cpp
)

add_library(pipeline
    src/pipeline.cpp
)

add_library(cli_converter
    src/pipeline_converter.cpp
)

add_library(application
    src/application.cpp
)

target_include_directories(parser PUBLIC include)
target_include_directories(waveform PUBLIC include)
target_include_directories(wav_reader PUBLIC include)
target_include_directories(wav_writer PUBLIC include)
target_include_directories(filters PUBLIC include)
target_include_directories(pipeline PUBLIC include)
target_include_directories(cli_converter PUBLIC include)
target_include_directories(application PUBLIC include)

target_link_libraries(filters PUBLIC waveform)
target_link_libraries(pipeline PUBLIC waveform)
target_link_libraries(cli_converter PUBLIC pipeline parser)
target_link_libraries(application
    PUBLIC
    parser
    waveform
    wav_reader
    wav_writer
    cli_converter
    filters
)

# ─────────────────────────────
# APP
# ─────────────────────────────

add_executable(sound_processor
    src/main.cpp
)

target_link_libraries(sound_processor application)

# ─────────────────────────────
# CATCH2
# ─────────────────────────────

include(CTest)

if(BUILD_TESTING)
    include(FetchContent)

    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.0
    )

    FetchContent_MakeAvailable(Catch2)

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
    # WAV READER TESTS
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

    # ─────────────────────────────
    # WAV WRITER TESTS
    # ─────────────────────────────

    add_executable(wav_writer_tests
        tests/test_wav_writer.cpp
    )

    target_link_libraries(wav_writer_tests
        wav_writer
        wav_reader
        waveform
        Catch2::Catch2WithMain
    )

    catch_discover_tests(wav_writer_tests)

    add_executable(filter_tests
        tests/test_filters.cpp
    )

    target_link_libraries(filter_tests
        filters
        Catch2::Catch2WithMain
    )

    catch_discover_tests(filter_tests)

    add_executable(converter_tests
        tests/test_converter.cpp
    )

    target_link_libraries(converter_tests
        cli_converter
        filters
        Catch2::Catch2WithMain
    )

    catch_discover_tests(converter_tests)

    add_executable(application_tests
        tests/test_application.cpp
    )

    target_link_libraries(application_tests
        application
        Catch2::Catch2WithMain
    )

    catch_discover_tests(application_tests)
endif()
