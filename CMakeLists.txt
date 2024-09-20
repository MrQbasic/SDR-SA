cmake_minimum_required(VERSION 3.13)

project(SDRSA)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g")

include_directories(${CMAKE_SOURCE_DIR}/lib)
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_subdirectory(lib)

add_subdirectory(src)

target_link_options(SDRSA PRIVATE -lglfw -lGL -ldl -lrtlsdr -lfftw3)