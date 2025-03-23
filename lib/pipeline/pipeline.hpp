#include "mod-lib/pipelineBlock.hpp"

#include <vector>

typedef pipelineBlock* (*CreateModuleFunc_pipline)();
std::vector<CreateModuleFunc_pipline> piplineBlock_createFuncs;


void setupPipline(){
    std::string path = "./build/modules/source";
    try {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            std::string path = entry.path();
            //check if its even the file type we need!
            if(path.find(".so") == std::string::npos) continue;
            if(path.find("piplineBlock_") == std::string::npos) continue;
            //try loading the module   
            void* handle = dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
            if(!handle){
                std::cerr << "Faild to load Module: " << path << std::endl;
                std::cerr <<  dlerror() << std::endl;
                continue;
            }
            dlerror();
            //getting the factory function

            CreateModuleFunc_pipline createFunc = (CreateModuleFunc_pipline) dlsym(handle, "moduleCreate_pipelineBlock");
            if (!createFunc) {
                std::cerr << "Failed to get createModule function: " << dlerror() << "\n";
                dlclose(handle);
                continue;
            }
            piplineBlock_createFuncs.push_back(createFunc);

            std::cout << "Module added: " << path << std::endl;
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error opening directory: " << e.what() << std::endl;
    }
}