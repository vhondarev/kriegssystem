cmake_minimum_required(VERSION 4.0)

project(MyCProject LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")

# add_library(shared_funcs STATIC
#     src/func1.c
#     src/func2.c
#     src/func3.c
# )

add_executable(
    ground-engine
    src/ground-engine.c
)

target_link_libraries(ground-engine PRIVATE
    ${CMAKE_SOURCE_DIR}/c-algs-and-ds-lib/bin/lib/algds.a
    # shared_funcs # Link against the newly created shared_funcs library
)

target_include_directories(ground-engine PRIVATE
    ${CMAKE_SOURCE_DIR}/c-algs-and-ds-lib/bin/lib/include/*.h
)

add_executable(
    naval-engine
    src/naval-engine.c
)

target_link_libraries(naval-engine PRIVATE
    ${CMAKE_SOURCE_DIR}/c-algs-and-ds-lib/bin/lib/algds.a
    # shared_funcs # Link against the newly created shared_funcs library
)

target_include_directories(naval-engine PRIVATE
    ${CMAKE_SOURCE_DIR}/c-algs-and-ds-lib/bin/lib/include/*.h

)

# cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

