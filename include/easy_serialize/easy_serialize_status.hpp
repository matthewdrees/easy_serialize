#pragma once

#include <string>

namespace easy_serialize
{
    // Return status for to/from operations
    struct EasySerializeStatus
    {
        std::string error_message{""};

        operator bool() const { return error_message.empty(); }
    };
}