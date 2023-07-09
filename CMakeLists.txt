# Header
cmake_minimum_required(VERSION 3.22)
project("ktane" VERSION 0.1.0 LANGUAGES CXX)

# Options
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Paths
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(SRC_PATH "${PROJECT_SOURCE_DIR}/src")
set(INCLUDE_PATH "${PROJECT_SOURCE_DIR}/include")

# Targets
# wires
file(GLOB_RECURSE WIRES_HEADERS CONFIGURE_DEPENDS "${INCLUDE_PATH}/getchar_sane.hpp")
file(GLOB_RECURSE WIRES_SOURCES CONFIGURE_DEPENDS "${SRC_PATH}/wires.cpp" "${SRC_PATH}/getchar_sane.cpp")
add_executable("wires" ${WIRES_SOURCES} ${WIRES_HEADERS})
target_include_directories("wires" PRIVATE "${INCLUDE_PATH}")

# button
file(GLOB_RECURSE BUTTON_HEADERS CONFIGURE_DEPENDS "${INCLUDE_PATH}/getchar_sane.hpp")
file(GLOB_RECURSE BUTTON_SOURCES CONFIGURE_DEPENDS "${SRC_PATH}/button.cpp" "${SRC_PATH}/getchar_sane.cpp")
add_executable("button" ${BUTTON_SOURCES} ${BUTTON_HEADERS})
target_include_directories("button" PRIVATE "${INCLUDE_PATH}")

# keypad
file(GLOB_RECURSE KEYPAD_SOURCES CONFIGURE_DEPENDS "${SRC_PATH}/keypad.cpp")
add_executable("keypad" ${KEYPAD_SOURCES})

# simon-says
file(GLOB_RECURSE SIMON_SAYS_HEADERS CONFIGURE_DEPENDS "${INCLUDE_PATH}/getchar_sane.hpp")
file(GLOB_RECURSE SIMON_SAYS_SOURCES CONFIGURE_DEPENDS "${SRC_PATH}/simon_says.cpp" "${SRC_PATH}/getchar_sane.cpp")
add_executable("simon-says" ${SIMON_SAYS_SOURCES} ${SIMON_SAYS_HEADERS})
target_include_directories("simon-says" PRIVATE "${INCLUDE_PATH}")

# whos-on-first
file(GLOB_RECURSE WHOS_ON_FIRST_HEADERS CONFIGURE_DEPENDS "${INCLUDE_PATH}/getchar_sane.hpp")
file(GLOB_RECURSE WHOS_ON_FIRST_SOURCES CONFIGURE_DEPENDS "${SRC_PATH}/whos_on_first.cpp" "${SRC_PATH}/getchar_sane.cpp")
add_executable("whos-on-first" ${WHOS_ON_FIRST_SOURCES} ${WHOS_ON_FIRST_HEADERS})
target_include_directories("whos-on-first" PRIVATE "${INCLUDE_PATH}")

# 3rd Party Libraries
find_package(fmt CONFIG REQUIRED)
target_link_libraries("wires" PRIVATE fmt::fmt)
target_link_libraries("button" PRIVATE fmt::fmt)
target_link_libraries("keypad" PRIVATE fmt::fmt)
target_link_libraries("simon-says" PRIVATE fmt::fmt)
target_link_libraries("whos-on-first" PRIVATE fmt::fmt)

# Precompiled headers
target_precompile_headers("wires" PRIVATE)
target_precompile_headers("button" PRIVATE)
target_precompile_headers("keypad" PRIVATE)
target_precompile_headers("simon-says" PRIVATE)
target_precompile_headers("whos-on-first" PRIVATE)

# CCache
find_program(CCACHE_FOUND ccache)

if(CCACHE_FOUND)
    message(STATUS "ccache found: ${MOLD_EXECUTABLE}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

# Compiler flags
if(MSVC)
    # Compiling with Microsoft Visual C++.
    list(APPEND WARNING_FLAGS "/W4")
else()
    # Compiling with g++/clang++/etc.
    list(
        APPEND WARNING_FLAGS
        "-pedantic-errors"
        "-Wfatal-errors"
        "-Werror=pedantic"
        "-Wall"
        "-Wextra"
        "-Wdouble-promotion"
        "-Wconversion"
        "-Wsign-conversion"
    )
endif()
target_compile_options("wires" PRIVATE ${WARNING_FLAGS})
target_compile_options("button" PRIVATE ${WARNING_FLAGS})
target_compile_options("keypad" PRIVATE ${WARNING_FLAGS})
target_compile_options("simon-says" PRIVATE ${WARNING_FLAGS})
target_compile_options("whos-on-first" PRIVATE ${WARNING_FLAGS})

# Mold Linker
find_program(MOLD_EXECUTABLE "mold")

if(MOLD_EXECUTABLE)
    message(STATUS "mold linker found: ${MOLD_EXECUTABLE}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=mold")
else()
    message(STATUS "mold linker not found.")
endif()

# Linker Flags
set_target_properties("wires" PROPERTIES LINK_FLAGS_RELEASE -s)
set_target_properties("button" PROPERTIES LINK_FLAGS_RELEASE -s)
set_target_properties("keypad" PROPERTIES LINK_FLAGS_RELEASE -s)
set_target_properties("simon-says" PROPERTIES LINK_FLAGS_RELEASE -s)
set_target_properties("whos-on-first" PROPERTIES LINK_FLAGS_RELEASE -s)
message(STATUS "Release build binary striping enabled.")

# Link-Time-Optimization
include(CheckIPOSupported)

check_ipo_supported(RESULT LTO_SUPPORTED OUTPUT USE_LTO_ERROR)

if(LTO_SUPPORTED)
    message(STATUS "LTO enabled")
    set_property(TARGET "wires" PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_property(TARGET "button" PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_property(TARGET "keypad" PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_property(TARGET "simon-says" PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_property(TARGET "whos-on-first" PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(STATUS "LTO not supported")
endif()
