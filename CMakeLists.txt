cmake_minimum_required(VERSION 3.22)
project(STS LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Catch2 3 REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wpedantic -Wconversion")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if(CMAKE_BUILD_TYPE MATCHES DEBUG)
  add_compile_options(-fsanitize=address,undefined)
  add_link_options(-fsanitize=address,undefined)
endif()

include_directories("src")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# revsynth_lib #
file(GLOB_RECURSE revsynth_src "src/*.cpp")
list(FILTER revsynth_src EXCLUDE REGEX ".*_ut\\.cpp$")
add_library(revsynth_lib STATIC ${revsynth_src})

# revsynth_lib unit tests#
file(GLOB_RECURSE ut_src "src/*_ut.cpp")
add_executable(revsynth_ut ${ut_src})
target_link_libraries(revsynth_ut PRIVATE revsynth_lib)
target_link_libraries(revsynth_ut PRIVATE Catch2::Catch2WithMain)

# # STS.EXE #
# file(GLOB main_src "src/main.cpp")
# add_executable(sts ${main_src})
# target_link_libraries(sts PRIVATE sts_lib)
