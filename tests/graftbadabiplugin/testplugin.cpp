#include "testpluginapi.h"


namespace {


int gimmeFive()
{
    return 5;
}


void setValue(const char* value)
{
}


const char* cValue()
{
    return "";
}


bool onCommand(const char* node, const char* data, unsigned int size)
{
    return true;
}


const char* lastError()
{
    return nullptr;
}


const icy::graft::testplugin::Api API = {
    setValue,
    cValue,
    onCommand,
    lastError,
    gimmeFive,
};


} // namespace


extern "C" {


ICY_GRAFT_EXPORT extern const icy::graft::Manifest icy_graft_manifest = {
    icy::graft::ABI_VERSION + 1,
    __FILE__,
    "bad-abi-plugin",
    "Bad ABI Plugin",
    "0.1.0",
    icy::graft::RUNTIME_NATIVE,
    icy::graft::testplugin::ENTRYPOINT,
};


ICY_GRAFT_EXPORT const icy::graft::testplugin::Api* icy_graft_test_api()
{
    return &API;
}


}
