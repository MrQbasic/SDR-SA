cmake_minimum_required(VERSION 3.10)

project(SDRSA)
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/lib)
link_directories(${CMAKE_SOURCE_DIR}/lib)

#ImGui
file(GLOB_RECURSE ImGui_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/lib/ImGui/*.cpp")
add_executable(SDRSA ${ImGui_SOURCES})

#main
add_executable(SDRSA src/SDRSA.cpp)

#imgui