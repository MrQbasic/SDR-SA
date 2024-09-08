cmake_minimum_required(VERSION 3.13)

project(SDRSA)
set(CMAKE_CXX_STANDARD 17)


include_directories(${CMAKE_SOURCE_DIR}/lib)
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_subdirectory(lib)

add_subdirectory(src)

target_link_options(SDRSA PRIVATE -lglfw -lGL -ldl)