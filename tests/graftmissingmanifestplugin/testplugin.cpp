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


extern "C" ICY_GRAFT_EXPORT const icy::graft::testplugin::Api* icy_graft_test_api()
{
    return &API;
}
