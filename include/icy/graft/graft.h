///
//
// icey
// Copyright (c) 2005, icey <https://0state.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @defgroup graft Graft module
///
/// Native ABI contracts and runtime loading helpers.
/// @{


#pragma once


#include "icy/graft/abi.h"

#include "icy/base.h"
#include "icy/sharedlibrary.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>


// Shared library exports
#if defined(Graft_EXPORTS)
#define Graft_API ICY_EXPORT
#else
#define Graft_API ICY_IMPORT
#endif

namespace icy {
/// @ingroup graft
/// Shared-library ABI contracts and runtime loading helpers.
namespace graft {


/// Current binary manifest ABI version required by the loader.
inline constexpr std::uint32_t ABI_VERSION = ICY_GRAFT_ABI_VERSION;
/// Exported symbol name that plugin libraries use for their manifest.
inline constexpr const char* PLUGIN_MANIFEST_SYMBOL = ICY_GRAFT_PLUGIN_MANIFEST_SYMBOL;
/// Backwards-compatible name for `PLUGIN_MANIFEST_SYMBOL`.
inline constexpr const char* MANIFEST_SYMBOL = PLUGIN_MANIFEST_SYMBOL;
/// Runtime string for plugins loaded directly into the current process.
inline constexpr const char* RUNTIME_NATIVE = ICY_GRAFT_RUNTIME_NATIVE;
/// Runtime string for plugins intended to execute in a worker runtime.
inline constexpr const char* RUNTIME_WORKER = ICY_GRAFT_RUNTIME_WORKER;
/// Runtime string for host-exported C ABI surfaces.
inline constexpr const char* RUNTIME_HOST = ICY_GRAFT_RUNTIME_HOST;


/// Runtime contract declared by a graft manifest.
enum class RuntimeKind
{
    /// Runtime string is missing or not recognized.
    Unknown = 0,
    /// Plugin is loaded directly into the host process.
    Native,
    /// Plugin is intended for a worker runtime.
    Worker,
    /// Manifest describes a host-exported C ABI surface, not a plugin.
    Host,
};


/// Metadata exported by a plugin or host surface.
struct Manifest
{
    /// ABI version expected to match `ABI_VERSION`.
    std::uint32_t abiVersion;
    /// Source file that declared the manifest.
    const char* fileName;
    /// Stable plugin or surface identifier.
    const char* id;
    /// Human-readable plugin or surface name.
    const char* name;
    /// Plugin or surface version string.
    const char* version;
    /// Runtime contract string, such as `native`, `worker`, or `host`.
    const char* runtime;
    /// Exported symbol name for the typed entrypoint.
    const char* entrypoint;
};

static_assert(sizeof(Manifest) == sizeof(::icy_graft_manifest_t));
static_assert(alignof(Manifest) == alignof(::icy_graft_manifest_t));
static_assert(offsetof(Manifest, abiVersion) == offsetof(::icy_graft_manifest_t, abiVersion));
static_assert(offsetof(Manifest, fileName) == offsetof(::icy_graft_manifest_t, fileName));
static_assert(offsetof(Manifest, id) == offsetof(::icy_graft_manifest_t, id));
static_assert(offsetof(Manifest, name) == offsetof(::icy_graft_manifest_t, name));
static_assert(offsetof(Manifest, version) == offsetof(::icy_graft_manifest_t, version));
static_assert(offsetof(Manifest, runtime) == offsetof(::icy_graft_manifest_t, runtime));
static_assert(offsetof(Manifest, entrypoint) == offsetof(::icy_graft_manifest_t, entrypoint));


/// Loads a native plugin library and resolves its typed entrypoint.
class Graft_API Library
{
public:
    Library() = default;
    ~Library() noexcept;

    Library(const Library&) = delete;
    Library& operator=(const Library&) = delete;
    Library(Library&&) = delete;
    Library& operator=(Library&&) = delete;

    void open(const std::string& path);
    void close() noexcept;

    [[nodiscard]] bool isOpen() const noexcept;
    [[nodiscard]] const std::string& path() const noexcept;
    [[nodiscard]] const Manifest& manifest() const;

    [[nodiscard]] void* requireSymbolAddress(const char* name) const;

    template <typename T>
    [[nodiscard]] T requireSymbol(const char* name) const
    {
        return reinterpret_cast<T>(requireSymbolAddress(name));
    }

    template <typename T>
    [[nodiscard]] T entrypoint() const
    {
        return requireSymbol<T>(manifest().entrypoint);
    }

private:
    void loadManifest();

    mutable SharedLibrary _library;
    std::string _path;
    const Manifest* _manifest = nullptr;
    bool _open = false;
};


/// Converts a manifest runtime string to a `RuntimeKind`.
[[nodiscard]] Graft_API RuntimeKind parseRuntimeKind(std::string_view runtime) noexcept;
/// Returns the manifest runtime string for a `RuntimeKind`.
[[nodiscard]] Graft_API const char* runtimeKindName(RuntimeKind runtime) noexcept;
/// Throws when a manifest is incompatible or missing required fields.
Graft_API void validateManifest(const Manifest& manifest, std::string_view path);
/// Throws when a manifest is not a plugin-loadable manifest.
Graft_API void validatePluginManifest(const Manifest& manifest, std::string_view path);
/// Throws when a manifest is not a host-exported surface manifest.
Graft_API void validateHostSurfaceManifest(const Manifest& manifest, std::string_view path);

[[nodiscard]] inline Manifest manifestFromC(const ::icy_graft_manifest_t& manifest) noexcept
{
    return {
        manifest.abiVersion,
        manifest.fileName,
        manifest.id,
        manifest.name,
        manifest.version,
        manifest.runtime,
        manifest.entrypoint,
    };
}

inline void validateManifest(const ::icy_graft_manifest_t& manifest, std::string_view path)
{
    validateManifest(manifestFromC(manifest), path);
}

inline void validatePluginManifest(const ::icy_graft_manifest_t& manifest, std::string_view path)
{
    validatePluginManifest(manifestFromC(manifest), path);
}

inline void validateHostSurfaceManifest(const ::icy_graft_manifest_t& manifest, std::string_view path)
{
    validateHostSurfaceManifest(manifestFromC(manifest), path);
}


} // namespace graft
} // namespace icy


#define ICY_GRAFT_PLUGIN(pluginId, pluginName, pluginVersion, runtimeKind, entrypointName) \
    extern "C" {                                                                            \
    ICY_GRAFT_EXPORT extern const icy::graft::Manifest icy_graft_manifest = {               \
        icy::graft::ABI_VERSION,                                                            \
        __FILE__,                                                                           \
        pluginId,                                                                           \
        pluginName,                                                                         \
        pluginVersion,                                                                      \
        runtimeKind,                                                                        \
        entrypointName,                                                                     \
    };                                                                                      \
    }


/// @}
