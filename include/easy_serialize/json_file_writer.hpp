// easy_serialize JSON helper functions for writing to a file.
#pragma once

#include "easy_serialize_status.hpp"
#include "json_writer.hpp"

#include <cstdio>

namespace easy_serialize
{

    // Helper function for writing json to a file.
    EasySerializeStatus to_file(const std::string &filename, const std::string &json)
    {
        EasySerializeStatus status;
        std::FILE *fp = fopen(filename.c_str(), "w");
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

    // Write object UTF-8 JSON to file.
    //
    // \param filename: "path/to/filename.json"
    // \param obj: object to write
    // \param json_indent: JSON indent formatting
    // \return status
    template <typename T>
    EasySerializeStatus to_json_file(const std::string &filename, T &obj,
                                     JsonIndent json_indent = JsonIndent::two_spaces)
    {
        std::string json = to_json_string(obj, json_indent);
        return to_file(filename, json);
    }

    // Write vector of objects UTF-8 JSON to a file.
    //
    // \param filename: "path/to/filename.json"
    // \param v: vector of objects to write
    // \param json_indent: JSON indent formatting
    // \return status
    template <typename T>
    EasySerializeStatus to_json_file_vector_objects(const std::string &filename, std::vector<T> &v,
                                                    JsonIndent json_indent = JsonIndent::two_spaces)
    {
        std::string json = to_json_string_vector_objects(v, json_indent);
        return to_file(filename, json);
    }

    // Write vector of enums UTF-8 JSON to a file.
    //
    // \param filename: "path/to/filename.json"
    // \param v: vector of enums to write
    // \param json_indent: JSON indent formatting
    // \return status
    template <typename T>
    EasySerializeStatus to_json_file_vector_enums(const std::string &filename, std::vector<T> &v,
                                                  JsonIndent json_indent = JsonIndent::two_spaces)
    {
        std::string json = to_json_string_vector_enums(v, json_indent);
        return to_file(filename, json);
    }

    // Write vector of supported C++ fundamental types UTF-8 JSON to a file.
    //
    // \param filename: "path/to/filename.json"
    // \param v: vector of enums to write
    // \param json_indent: JSON indent formatting
    // \return status
    template <typename T>
    EasySerializeStatus to_json_file_vector(const std::string &filename, std::vector<T> &v,
                                            JsonIndent json_indent = JsonIndent::two_spaces)
    {
        std::string json = to_json_string_vector(v, json_indent);
        return to_file(filename, json);
    }

} // namespace easy_serialize