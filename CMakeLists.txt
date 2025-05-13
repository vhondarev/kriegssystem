cmake_minimum_required(VERSION 4.0)

project(MyCProject LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/c-algs-and-ds-lib/bin/lib/include)

add_library(static STATIC
    src/static/utils.c
)
target_include_directories(static PUBLIC ${CMAKE_SOURCE_DIR}/src/static)

set_target_properties(static PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build
)

# add_executable(ground src/ground-engine.c)
#
# target_link_libraries(ground PRIVATE
#     ${CMAKE_SOURCE_DIR}/c-algs-and-ds-lib/bin/lib/libalgds.a
#     static
# )

add_executable(naval src/naval-engine.c)

target_link_libraries(naval PRIVATE
    ${CMAKE_SOURCE_DIR}/c-algs-and-ds-lib/bin/lib/libalgds.a
    ${CMAKE_SOURCE_DIR}/src/naval/vessels.c
    static
)

target_include_directories(static PUBLIC
    ${CMAKE_SOURCE_DIR}/src/naval
)

