cmake_minimum_required(VERSION 3.14)

project(sound_processor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOUND_PROCESSOR_SOURCES
    src/application.cpp
    src/parser.cpp
    src/pipeline.cpp
    src/pipeline_converter.cpp
    src/waveform.cpp
    src/wav_reader.cpp
    src/wav_writer.cpp
    src/filters/ampl.cpp
    src/filters/generators.cpp
    src/filters/lowpass.cpp
    src/filters/normalize.cpp
    src/filters/producers.cpp
    src/filters/silence.cpp
    src/filters/timestretch.cpp
)

add_executable(sound_processor
    src/main.cpp
    ${SOUND_PROCESSOR_SOURCES}
)

target_include_directories(sound_processor PRIVATE include)

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

    add_executable(test_sound_processor
        tests/test_application.cpp
        tests/test_converter.cpp
        tests/test_filters.cpp
        tests/test_parser.cpp
        tests/test_wav_reader.cpp
        tests/test_wav_writer.cpp
        tests/test_waveform.cpp
        ${SOUND_PROCESSOR_SOURCES}
    )

    target_include_directories(test_sound_processor PRIVATE include)
    target_link_libraries(test_sound_processor PRIVATE Catch2::Catch2WithMain)

    catch_discover_tests(test_sound_processor)
endif()
