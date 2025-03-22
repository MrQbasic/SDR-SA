#Find all modules
file(GLOB MODULES_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} */)

#go over every module
foreach(MODULE_DIR ${MODULES_DIRS})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_DIR})
        # Create a shared library for each module subdirectory
        file(GLOB MODULE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_DIR}/*.cpp)

        # Create the module as a shared library
        add_library(${MODULE_DIR} SHARED ${MODULE_SOURCES})
        set_target_properties(${MODULE_DIR} PROPERTIES PREFIX "")
        
        # Optionally, you can link specific libraries to each module here if needed
        # target_link_libraries(${MODULE_DIR} PRIVATE some_library)
    endif()
endforeach()