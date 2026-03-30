add_library(graftmissingmanifestplugin SHARED)
file(GLOB _srcs "*.cpp")
file(GLOB _hdrs "../grafttestplugin/*.h" "*.h")
target_sources(graftmissingmanifestplugin PRIVATE ${_srcs} ${_hdrs})
target_link_libraries(graftmissingmanifestplugin PRIVATE base graft)
target_include_directories(graftmissingmanifestplugin PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../grafttestplugin
)
