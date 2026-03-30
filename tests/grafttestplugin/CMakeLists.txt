# Plugin test library - built as a shared library for graft loading tests
add_library(grafttestplugin SHARED)
file(GLOB _srcs "*.cpp")
file(GLOB _hdrs "*.h*")
target_sources(grafttestplugin PRIVATE ${_srcs} ${_hdrs})
target_link_libraries(grafttestplugin PRIVATE base graft)
target_include_directories(grafttestplugin PRIVATE ${CMAKE_BINARY_DIR})
