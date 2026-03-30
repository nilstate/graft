icy_add_test(grafttests DEPENDS base graft)
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
