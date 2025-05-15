cmake_minimum_required(VERSION 4.0)

project(kriegssystem LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

add_library(static STATIC
  src/static/utils.c
)
target_include_directories(static PUBLIC
  ${CMAKE_SOURCE_DIR}/src/static
)
set_target_properties(static PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build
)

add_library(naval_lib STATIC
    src/naval/vessels.c
    src/naval/combat.c
    src/naval/prototypes.c
)
target_include_directories(naval_lib PUBLIC
    ${CMAKE_SOURCE_DIR}/src/naval
    ${CMAKE_SOURCE_DIR}/c-algs-and-ds-lib/bin/lib/include
)
set_target_properties(naval_lib PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build
)

add_executable(naval src/naval-engine.c)
target_link_libraries(naval PRIVATE
    ${CMAKE_SOURCE_DIR}/c-algs-and-ds-lib/bin/lib/libalgds.a
    naval_lib
    static
)
target_include_directories(naval PUBLIC
    ${CMAKE_SOURCE_DIR}/src/naval
    ${CMAKE_SOURCE_DIR}/c-algs-and-ds-lib/bin/lib/include
)
