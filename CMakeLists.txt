cmake_minimum_required(VERSION 3.23)

project(compilers)

# file(GLOB SRC_C *.c)
# file(GLOB SRC_H *.h)
#
# find_library(PAM_LIB pam)
# find_library(PAM_MISC_LIB pam_misc)

add_executable(parser1)

# target_link_libraries(dm PRIVATE ${PAM_LIB} ${PAM_MISC_LIB})

target_sources(parser1 PRIVATE parsers/simple1.c)

target_compile_options(parser1 PRIVATE -fsanitize=address -g)
target_link_options(parser1 PRIVATE -fsanitize=address)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -fno-omit-frame-pointer -g3 -O0")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined")
