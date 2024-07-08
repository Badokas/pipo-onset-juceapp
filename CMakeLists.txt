cmake_minimum_required(VERSION 3.24.1)

set(PROJECT_NAME "PipoOnsetApp")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/External/cmake-includes")

# Building universal binaries on macOS increases build time
if ((CMAKE_BUILD_TYPE STREQUAL "Release"))
    message("Building for Apple Silicon and x86_64")
    set(CMAKE_OSX_ARCHITECTURES arm64 x86_64)
else ()
    message("Building for Apple Silicon only")
    set(CMAKE_OSX_ARCHITECTURES arm64)
endif ()

# This must be set before the project() call
# see: https://cmake.org/cmake/help/latest/variable/CMAKE_OSX_DEPLOYMENT_TARGET.html
# FORCE must be set, see https://stackoverflow.com/a/44340246
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Support macOS down to High Sierra" FORCE)
# set(CMAKE_OSX_DEPLOYMENT_TARGET "13" CACHE STRING "Minimum macOS version required")

project("${PROJECT_NAME}" VERSION 1.0.0)

set(formats #[["VST3"]] "Standalone")

option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)
if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_compile_options(-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options(-fcolor-diagnostics)
    endif ()
endif ()

if (APPLE)
    # list(APPEND formats "AU")
endif ()

set(V9ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(LIBV9_DIR ../libv9)
set(RODYK_DIR ../rodyk)
set(GROK_DIR ../grok)

add_subdirectory(External)

juce_add_gui_app("${PROJECT_NAME}"
        VERSION "1.0.0"
        COMPANY_NAME "VOID9"
        PLUGIN_MANUFACTURER_CODE "V9ap"
        PLUGIN_CODE "A002"
        FORMATS ${formats}
        COPY_PLUGIN_AFTER_BUILD TRUE
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE
        PRODUCT_NAME "${PROJECT_NAME}"
)

juce_generate_juce_header("${PROJECT_NAME}")

add_definitions(-DV9_EMUTYPE_OMEGA=1)
add_definitions(-DMONAI_EMULATOR=1)

# use MOLD linker with Clang
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_definitions(-DCMAKE_CXX_FLAGS="-fuse-ld=mold")
endif ()

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

file(GLOB SOURCES CONFIGURE_DEPENDS
        Source/Main.cpp
        Source/MainComponent.cpp
        Source/MainWindow.cpp
)

target_sources("${PROJECT_NAME}" PRIVATE ${SOURCES})

target_compile_features("${PROJECT_NAME}" PUBLIC cxx_std_20)

# Needed for TestPlugin dylib loading (easier dev)
set_target_properties("${PROJECT_NAME}"
        PROPERTIES
        BUILD_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/TestPlugin/build"
        INSTALL_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/TestPlugin/build"
)

add_subdirectory(TestPlugin)
add_dependencies("${PROJECT_NAME}" TestPlugin)

link_directories("${PROJECT_NAME}" External/haptic_macos/src/lib)

add_subdirectory(External/haptic_macos)
add_dependencies("${PROJECT_NAME}" HapticMacos)

target_link_libraries("${PROJECT_NAME}"
        PRIVATE
        lua
        juce_plugin_modules
        HapticMacos
)

message(STATUS "Forwarding CMake/Compiler info\n"
        "DEFAULT_MODULE_PATH=${CMAKE_CURRENT_SOURCE_DIR}/TestPlugin\n"
        "CMAKE_EXE_PATH=${CMAKE_COMMAND}\n"
        "CMAKE_GENERATOR=${CMAKE_GENERATOR}\n"
        "CMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}\n"
        "CMAKE_C_COMPILER=${CMAKE_C_COMPILER}\n"
        "CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
)

target_compile_definitions("${PROJECT_NAME}" PUBLIC
        DEFAULT_MODULE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/TestPlugin"
        CMAKE_EXE_PATH="${CMAKE_COMMAND}"
        CMAKE_GENERATOR="${CMAKE_GENERATOR}"
        CMAKE_MAKE_PROGRAM="${CMAKE_MAKE_PROGRAM}"
        CMAKE_C_COMPILER="${CMAKE_C_COMPILER}"
        CMAKE_CXX_COMPILER="${CMAKE_CXX_COMPILER}"
)

# Everything related to the tests target
# include(Tests)

# A separate target for Benchmarks (keeps the Tests target fast)
# include(Benchmarks)
