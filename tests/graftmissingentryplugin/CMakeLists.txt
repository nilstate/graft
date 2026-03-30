add_library(graftmissingentryplugin SHARED)
file(GLOB _srcs "*.cpp")
file(GLOB _hdrs "../grafttestplugin/*.h" "*.h")
target_sources(graftmissingentryplugin PRIVATE ${_srcs} ${_hdrs})
target_link_libraries(graftmissingentryplugin PRIVATE base graft)
target_include_directories(graftmissingentryplugin PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../grafttestplugin
)
