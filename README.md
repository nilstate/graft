# Graft

Native ABI contracts, host C surfaces, and shared-library loading for icey.

- Namespace: `icy::graft`
- CMake target: `icey::graft`
- Primary C++ header: `include/icy/graft/graft.h`
- Primary C ABI header: `include/icy/graft/abi.h`
- Directory layout: `include/` for the public ABI contract, `src/` for manifest validation and library loading, `tests/` for positive and negative shared-library fixtures

Graft is the native ABI boundary for icey. It defines:

- a versioned manifest exported from a shared library or host surface
- runtime kinds (`native`, `worker`, `host`)
- a loader that opens the library, validates the manifest, and resolves the declared entrypoint
- per-surface host-out C headers such as `icy/graft/host/pipeline.h`

Graft does not make a C++ class hierarchy the public ABI. Hosts, plugins, and foreign bindings agree on a manifest and then exchange typed C-compatible entrypoints.

- [Graft module guide](../../docs/modules/graft.md)
