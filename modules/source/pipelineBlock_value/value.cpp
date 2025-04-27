#include "mod-lib/pipelineBlock.hpp"



class pipelineBlock_value : public pipelineBlock{
public:
    pipelineBlock_value(){
        this->headline = "Value";
    }

    void run() override {}

    std::string getModuleName() override {return "pipelineBlock_value";}

    PinType getPinType(int id) override{
        switch(id){
            case 50:
                return PinType_value;
            default:
                return PinType_none;
        }
    }
    
    void* getBuffer(int index){
        switch(index){
            case 0:
                return &this->value;
            default:
                return nullptr;
        }
    }
    

    bool render() override {
        generic_renderStart();
        ImGui::SetNextItemWidth(100);
        setNodeStyle(PinType_value);
        ImNodes::BeginOutputAttribute(this->nodeID*100 + 50);
        ImGui::InputDouble("###valueinput", &this->value);
        ImNodes::EndOutputAttribute();
        ImNodes::PopColorStyle();
        return generic_renderEnd();
    }


    bool renderAdd() override{
        return ImGui::MenuItem("Value");
    }

private:
    double value = 0;

};


extern "C" pipelineBlock* moduleCreate_pipelineBlock() {
    return (pipelineBlock*) new pipelineBlock_value();
}