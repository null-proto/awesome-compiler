cmake_minimum_required(VERSION 3.23)

project(compilers)

file(GLOB INCLUDE_C include/*.c)
file(GLOB INCLUDE_H include/*.h)


file(GLOB PARSER2_C parsers/simple2/*.c)
file(GLOB PARSER2_H parsers/simple2/*.h)

# find_library(FMT_LIB fmt)
# find_library(PAM_MISC_LIB pam_misc)

add_executable(parser1)
add_executable(parser2)

# target_link_libraries(parser1 PRIVATE ${FMT})

target_sources(parser1 PRIVATE parsers/simple1.c)
target_sources(parser2 PRIVATE ${PARSER2_C} ${PARSER2_H})

target_compile_options(parser1 PRIVATE -fsanitize=address -g)
target_link_options(parser1 PRIVATE -fsanitize=address)

target_compile_options(parser2 PRIVATE -fsanitize=address -g)
target_link_options(parser2 PRIVATE -fsanitize=address)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -fno-omit-frame-pointer -g3 -O0")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined")
