add_library(graftbadabiplugin SHARED)
file(GLOB _srcs "*.cpp")
file(GLOB _hdrs "../grafttestplugin/*.h" "*.h")
target_sources(graftbadabiplugin PRIVATE ${_srcs} ${_hdrs})
target_link_libraries(graftbadabiplugin PRIVATE base graft)
target_include_directories(graftbadabiplugin PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../grafttestplugin
)
