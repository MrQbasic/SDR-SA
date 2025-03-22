file(GLOB_RECURSE MOD_LIB_SRC CONFIGURE_DEPENDS "mod-lib/*.cpp")
add_library(lModule ${MOD_LIB_SRC})

message(STATUS "${MOD_LIB_SRC}")

add_subdirectory(source)
