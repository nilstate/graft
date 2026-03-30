#pragma once


#include "icy/graft/graft.h"


namespace icy {
namespace graft {
namespace testplugin {


struct Api
{
    void (*setValue)(const char* value);
    const char* (*cValue)();
    bool (*onCommand)(const char* node, const char* data, unsigned int size);
    const char* (*lastError)();
    int (*gimmeFive)();
};


using GetApiFunc = const Api* (*)();

inline constexpr const char* ENTRYPOINT = "icy_graft_test_api";


} // namespace testplugin
} // namespace graft
} // namespace icy
