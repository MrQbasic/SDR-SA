cmake_minimum_required(VERSION 3.13)

project(SDRSA)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_EXE_LINKER_FLAGS "-rdynamic")



include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${CMAKE_SOURCE_DIR}/modules)
include_directories(${CMAKE_SOURCE_DIR}/lib-ext)
include_directories(${CMAKE_SOURCE_DIR}/lib-ext/ImGui)
include_directories(${CMAKE_SOURCE_DIR}/lib-ext/implot)
#link_directories(${CMAKE_SOURCE_DIR}/lib)
#link_directories(${CMAKE_SOURCE_DIR}/modules)


add_subdirectory(lib-ext)
add_subdirectory(lib)
add_subdirectory(modules)

add_subdirectory(src)

target_link_options(SDRSA PRIVATE -lglfw -lGL -ldl -lbladeRF -lrtlsdr -lfftw3 -Wl,--export-dynamic)

