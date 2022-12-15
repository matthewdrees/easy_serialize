#pragma once

#include <string>

namespace easy_serialize
{
    // Return status for to/from operations
    class EasySerializeStatus
    {
        std::string _error_message{""};

    public:
        operator bool() const noexcept { return _error_message.empty(); }
        std::string get_error_message() const noexcept { return _error_message; }
        void set_error_message(std::string error_message) { _error_message = error_message; }
    };
}