#include "icy/graft/graft.h"
#include "icy/platform.h"
#include "icy/test.h"

#include "grafttestplugin/testpluginapi.h"

#include <cstring>
#include <stdexcept>
#include <string>


using namespace icy;
using namespace icy::test;


namespace {


std::string pluginPath(const std::string& folder, const std::string& target)
{
    std::string path(ICY_BUILD_DIR);
    path += "/graft/tests/";
    path += folder;
    path += "/";

#if WIN32
    path += target + GRAFT_TEST_PLUGIN_DEBUG_POSTFIX + GRAFT_TEST_PLUGIN_SUFFIX;
#else
    path += GRAFT_TEST_PLUGIN_PREFIX + target + GRAFT_TEST_PLUGIN_DEBUG_POSTFIX
          + GRAFT_TEST_PLUGIN_SUFFIX;
#endif

    return path;
}


} // namespace


int main(int argc, char** argv)
{
    test::init();

    describe("valid graft plugin", []() {
        graft::Library library;
        library.open(pluginPath("grafttestplugin", "grafttestplugin"));

        expect(library.isOpen());
        expect(library.manifest().abiVersion == graft::ABI_VERSION);
        expect(std::strcmp(library.manifest().id, "test-plugin") == 0);
        expect(std::strcmp(library.manifest().runtime, graft::RUNTIME_NATIVE) == 0);
        expect(graft::parseRuntimeKind(library.manifest().runtime) == graft::RuntimeKind::Native);

        auto api = library.entrypoint<graft::testplugin::GetApiFunc>()();
        expect(api != nullptr);
        api->setValue("abracadabra");
        expect(std::strcmp(api->cValue(), "abracadabra") == 0);
        expect(api->onCommand("options:set", "randomdata", 10));
        expect(api->lastError() == nullptr);
        expect(api->onCommand("unknown:command", "randomdata", 10) == false);
        expect(std::strcmp(api->lastError(), "Unknown command") == 0);
        expect(api->gimmeFive() == 5);

        library.close();
        expect(!library.isOpen());
    });

    describe("missing manifest rejected", []() {
        bool threw = false;
        try {
            graft::Library library;
            library.open(pluginPath("graftmissingmanifestplugin", "graftmissingmanifestplugin"));
        } catch (const std::runtime_error&) {
            threw = true;
        }
        expect(threw);
    });

    describe("abi mismatch rejected", []() {
        bool threw = false;
        try {
            graft::Library library;
            library.open(pluginPath("graftbadabiplugin", "graftbadabiplugin"));
        } catch (const std::runtime_error&) {
            threw = true;
        }
        expect(threw);
    });

    describe("missing entrypoint rejected", []() {
        bool threw = false;
        try {
            graft::Library library;
            library.open(pluginPath("graftmissingentryplugin", "graftmissingentryplugin"));
            library.entrypoint<graft::testplugin::GetApiFunc>()();
        } catch (const std::runtime_error&) {
            threw = true;
        }
        expect(threw);
    });

    describe("runtime parsing", []() {
        expect(graft::parseRuntimeKind(graft::RUNTIME_NATIVE) == graft::RuntimeKind::Native);
        expect(graft::parseRuntimeKind(graft::RUNTIME_WORKER) == graft::RuntimeKind::Worker);
        expect(graft::parseRuntimeKind("bogus") == graft::RuntimeKind::Unknown);
        expect(std::strcmp(graft::runtimeKindName(graft::RuntimeKind::Worker), graft::RUNTIME_WORKER) == 0);
    });

    test::runAll();
    return test::finalize();
}
