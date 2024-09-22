#include <ImGui/imgui.h>

#include <source/source.hpp>
#include <source/modifier/scanner.hpp>


void renderAddModifier(){
    if(ImGui::Selectable("Scanner", false, ImGuiSelectableFlags_DontClosePopups)){
        Source_Scanner* scanner = new Source_Scanner();
        Source::addSource(scanner);
    }
}