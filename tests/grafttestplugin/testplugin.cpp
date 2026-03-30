#include "testplugin.h"

#include <cstring>
#include <fstream>
#include <stdexcept>
#include <string>


namespace {


std::string gError;
std::string gValue;


void setValue(const char* value)
{
    gValue = value ? value : "";
}


const char* cValue()
{
    return gValue.c_str();
}


bool onCommand(const char* node, const char* data, unsigned int size)
{
    gError.clear();

    try {
        if (std::strcmp(node, "options:set") == 0)
            return true;

        if (std::strcmp(node, "file:write") == 0) {
            std::ofstream output("test.bin", std::ios::out | std::ios::binary);
            if (!output.is_open())
                throw std::runtime_error("Cannot write to output file");
            output.write(data, size);
            return true;
        }

        throw std::runtime_error("Unknown command");
    } catch (const std::exception& exc) {
        gError = exc.what();
        return false;
    }
}


const char* lastError()
{
    return gError.empty() ? nullptr : gError.c_str();
}


int gimmeFive()
{
    return 5;
}


const icy::graft::testplugin::Api API = {
    setValue,
    cValue,
    onCommand,
    lastError,
    gimmeFive,
};


} // namespace


ICY_GRAFT_PLUGIN("test-plugin",
                 "Test Plugin",
                 "0.1.1",
                 icy::graft::RUNTIME_NATIVE,
                 icy::graft::testplugin::ENTRYPOINT)


const icy::graft::testplugin::Api* testPluginApi()
{
    return &API;
}


extern "C" ICY_GRAFT_EXPORT const icy::graft::testplugin::Api* icy_graft_test_api()
{
    return testPluginApi();
}
