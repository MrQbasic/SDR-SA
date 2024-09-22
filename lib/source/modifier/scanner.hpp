#include <source/source.hpp>
#include <ImGui/imgui.h>

class Source_Scanner : public Source{
public:
    Source_Scanner(){
        this->name = new char[30] {"new Scanner"};
    }

    ~Source_Scanner(){
        delete this->name;
    }

    const char* getName() const override {
        return (const char*) name;
    }

    void updateData(long long centerFreq) override {

    }

    void renderMenu() override {
        

        ImGui::InputText("Name", name, 30, ImGuiInputTextFlags_None);
        ImGui::SetKeyboardFocusHere();
        
    }


private:
    char* name;

};