#include <ImGui/imgui.h>

#include <tools/oscFinder.hpp>

bool menu_render_tools_oscFinder = false;

void renderToolsMenu(){
    ImGui::Checkbox("Oscilator finder", &menu_render_tools_oscFinder);

}


void renderTools(){
    if(menu_render_tools_oscFinder) renderTool_OscFinder();
}