#pragma once

#include "json_reader.hpp"

namespace easy_serialize
{
    // Helper function for writing json to a file.
    EasySerializeStatus from_file(const std::string &filename, const std::string &json)
    {
        EasySerializeStatus status;
        std::FILE *fp = fopen(filename.c_str(), "r");
        if (!fp)
        {
            status.set_error_message("File opening failed.");
        }
        else
        {
            std::fwrite(json.c_str(), json.size(), 1, fp);
            fclose(fp);
        }
        return status;
    }


    // Populate object T
    template <typename T>
    EasySerializeStatus from_json_file(const std::string &filename, T &obj)
    {
        return from_json_buffer(BufferPtr buffer_ptr, size_t buffer_size, T & obj)
    }

    // Populate object T
    template <typename T>
    EasySerializeStatus from_json_file(const std::string &filename, std::vector<T> &v)
    {
    }

} // namespace easy_serialize
