///
//
// icey
// Copyright (c) 2005, icey <https://0state.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @defgroup graft Graft module
///
/// Native plugin ABI contract and runtime loading helpers.
/// @{


#pragma once


#include "icy/base.h"
#include "icy/sharedlibrary.h"

#include <cstdint>
#include <string>
#include <string_view>


// Shared library exports
#if defined(Graft_EXPORTS)
#define Graft_API ICY_EXPORT
#else
#define Graft_API ICY_IMPORT
#endif

#ifdef ICY_WIN
#define ICY_GRAFT_EXPORT __declspec(dllexport)
#elif defined(__GNUC__) || defined(__clang__)
#define ICY_GRAFT_EXPORT __attribute__((visibility("default")))
#else
#define ICY_GRAFT_EXPORT
#endif


namespace icy {
/// @ingroup graft
/// Shared-library plugin contracts and runtime loading helpers.
namespace graft {


/// Current binary manifest ABI version required by the loader.
inline constexpr std::uint32_t ABI_VERSION = 1;
/// Exported symbol name that plugins use for their manifest.
inline constexpr const char* MANIFEST_SYMBOL = "icy_graft_manifest";
/// Runtime string for plugins loaded directly into the current process.
inline constexpr const char* RUNTIME_NATIVE = "native";
/// Runtime string for plugins intended to execute in a worker runtime.
inline constexpr const char* RUNTIME_WORKER = "worker";


/// Runtime contract declared by a plugin manifest.
enum class RuntimeKind
{
    /// Runtime string is missing or not recognized.
    Unknown = 0,
    /// Plugin is loaded directly into the host process.
    Native,
    /// Plugin is intended for a worker runtime.
    Worker,
};


/// Metadata exported by a plugin under `icy_graft_manifest`.
struct Manifest
{
    /// ABI version expected to match `ABI_VERSION`.
    std::uint32_t abiVersion;
    /// Source file that declared the manifest.
    const char* fileName;
    /// Stable plugin identifier.
    const char* id;
    /// Human-readable plugin name.
    const char* name;
    /// Plugin version string.
    const char* version;
    /// Runtime contract string, such as `native` or `worker`.
    const char* runtime;
    /// Exported symbol name for the plugin entrypoint.
    const char* entrypoint;
};


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


} // namespace graft
} // namespace icy


#define ICY_GRAFT_PLUGIN(pluginId, pluginName, pluginVersion, runtimeKind, entrypointName) \
    extern "C" {                                                                             \
    ICY_GRAFT_EXPORT extern const icy::graft::Manifest icy_graft_manifest = {               \
        icy::graft::ABI_VERSION,                                                             \
        __FILE__,                                                                            \
        pluginId,                                                                            \
        pluginName,                                                                          \
        pluginVersion,                                                                       \
        runtimeKind,                                                                         \
        entrypointName,                                                                      \
    };                                                                                       \
    }


/// @}
