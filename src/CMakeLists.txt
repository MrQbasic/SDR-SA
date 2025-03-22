add_executable(SDRSA SDRSA.cpp)

#target_link_libraries(SDRSA PRIVATE lSDRSA)
target_link_libraries(SDRSA PRIVATE LIB_IMGUI)