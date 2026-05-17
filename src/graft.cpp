///
//
// icey
// Copyright (c) 2005, icey <https://0state.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup graft
/// @{


#include "icy/graft/graft.h"

#include <stdexcept>


namespace icy {
namespace graft {


Library::~Library() noexcept
{
    close();
}


void Library::open(const std::string& path)
{
    close();

    _library.open(path);
    _open = true;
    _path = path;

    try {
        loadManifest();
        validatePluginManifest(*_manifest, _path);
    } catch (...) {
        close();
        throw;
    }
}


void Library::close() noexcept
{
    if (_open)
        _library.close();

    _open = false;
    _manifest = nullptr;
    _path.clear();
}


bool Library::isOpen() const noexcept
{
    return _open;
}


const std::string& Library::path() const noexcept
{
    return _path;
}


const Manifest& Library::manifest() const
{
    if (!_open || !_manifest)
        throw std::runtime_error("graft library is not open");
    return *_manifest;
}


void* Library::requireSymbolAddress(const char* name) const
{
    if (!_open)
        throw std::runtime_error("graft library is not open");

    void* symbol = nullptr;
    _library.sym(name, &symbol);
    return symbol;
}


void Library::loadManifest()
{
    _manifest = requireSymbol<const Manifest*>(MANIFEST_SYMBOL);
}


RuntimeKind parseRuntimeKind(std::string_view runtime) noexcept
{
    if (runtime == RUNTIME_NATIVE)
        return RuntimeKind::Native;
    if (runtime == RUNTIME_WORKER)
        return RuntimeKind::Worker;
    if (runtime == RUNTIME_HOST)
        return RuntimeKind::Host;
    return RuntimeKind::Unknown;
}


const char* runtimeKindName(RuntimeKind runtime) noexcept
{
    switch (runtime) {
        case RuntimeKind::Native:
            return RUNTIME_NATIVE;
        case RuntimeKind::Worker:
            return RUNTIME_WORKER;
        case RuntimeKind::Host:
            return RUNTIME_HOST;
        default:
            return "unknown";
    }
}


void validateManifest(const Manifest& manifest, std::string_view path)
{
    if (manifest.abiVersion != ABI_VERSION)
        throw std::runtime_error("graft ABI mismatch in '" + std::string(path) + "'");
    if (!manifest.id || !*manifest.id)
        throw std::runtime_error("graft manifest is missing id in '" + std::string(path) + "'");
    if (!manifest.name || !*manifest.name)
        throw std::runtime_error("graft manifest is missing name in '" + std::string(path) + "'");
    if (!manifest.version || !*manifest.version)
        throw std::runtime_error("graft manifest is missing version in '" + std::string(path) + "'");
    if (!manifest.entrypoint || !*manifest.entrypoint)
        throw std::runtime_error("graft manifest is missing entrypoint in '" + std::string(path) + "'");
    if (parseRuntimeKind(manifest.runtime ? manifest.runtime : "") == RuntimeKind::Unknown)
        throw std::runtime_error("graft manifest has unknown runtime in '" + std::string(path) + "'");
}


void validatePluginManifest(const Manifest& manifest, std::string_view path)
{
    validateManifest(manifest, path);

    const auto runtime = parseRuntimeKind(manifest.runtime ? manifest.runtime : "");
    if (runtime == RuntimeKind::Host)
        throw std::runtime_error("graft manifest is a host surface, not a plugin, in '"
                                 + std::string(path) + "'");
}


void validateHostSurfaceManifest(const Manifest& manifest, std::string_view path)
{
    validateManifest(manifest, path);

    const auto runtime = parseRuntimeKind(manifest.runtime ? manifest.runtime : "");
    if (runtime != RuntimeKind::Host)
        throw std::runtime_error("graft manifest is not a host surface in '" + std::string(path) + "'");
}


} // namespace graft
} // namespace icy


/// @}
