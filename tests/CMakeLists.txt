icy_add_test(grafttests DEPENDS base graft)
target_compile_definitions(grafttests PRIVATE
  GRAFT_TEST_PLUGIN_PREFIX="${CMAKE_SHARED_LIBRARY_PREFIX}"
  GRAFT_TEST_PLUGIN_SUFFIX="${CMAKE_SHARED_LIBRARY_SUFFIX}"
  GRAFT_TEST_PLUGIN_DEBUG_POSTFIX="${icey_DEBUG_POSTFIX}"
)
add_subdirectory("grafttestplugin")
add_subdirectory("graftbadabiplugin")
add_subdirectory("graftmissingmanifestplugin")
add_subdirectory("graftmissingentryplugin")

add_dependencies(grafttests
  grafttestplugin
  graftbadabiplugin
  graftmissingmanifestplugin
  graftmissingentryplugin
)
