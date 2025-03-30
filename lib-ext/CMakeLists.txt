add_library(LIB_IMGUI SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGui/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGui/backends/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGui/backends/imgui_impl_opengl3.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/implot/implot.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/implot/implot_items.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/implot/implot_demo.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/imnodes/imnodes.cpp
)

target_include_directories(LIB_IMGUI PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ImGui)
target_include_directories(LIB_IMGUI PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/implot)
target_include_directories(LIB_IMGUI PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/imnodes)

target_link_options(LIB_IMGUI PRIVATE -lglfw -lGL -ldl)