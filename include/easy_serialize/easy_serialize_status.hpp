#pragma once

#include <string>

namespace easy_serialize
{
    // Return status and error message for some easy_serialize operations.
    //
    // Example:
    //     O object;
    //     const auto status = easy_serialize::from_json_string(json_string, object);
    //     if (status) {
    //         // Success. Do something with object.
    //     }
    //     else {
    //         std::cerr << status.get_error_message() << "\n";
    //     }
    class EasySerializeStatus
    {
        std::string _error_message{""};

    public:
        operator bool() const noexcept { return _error_message.empty(); }
        std::string get_error_message() const noexcept { return _error_message; }
        void set_error_message(std::string error_message) { _error_message = error_message; }
    };
}
