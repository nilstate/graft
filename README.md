# Graft

Native plugin ABI and shared-library loading for icey.

- Namespace: `icy::graft`
- CMake target: `icey::graft`
- Primary header: `include/icy/graft/graft.h`
- Directory layout: `include/` for the public ABI contract, `src/` for manifest validation and library loading, `tests/` for positive and negative shared-library fixtures

Graft is the native plugin boundary for icey. It defines:

- a versioned manifest exported from a shared library
- runtime kinds (`native`, `worker`)
- a loader that opens the library, validates the manifest, and resolves the declared entrypoint

Unlike the old `pluga` shape, graft does not make a C++ class hierarchy the public ABI. Hosts and plugins agree on a manifest and then exchange typed C-compatible entrypoints.

- [Graft module guide](../../docs/modules/graft.md)
