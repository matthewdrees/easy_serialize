#pragma once

#include "json_indent.hpp"
#include "rapidjsonwriter_impl.hpp"

#include <string>
#include <vector>

namespace easy_serialize
{
    // Write UTF-8 JSON to file.
    //
    // \param filename: "path/to/filename.json"
    // \param obj: object to write
    // \param json_indent: JSON indent formatting
    // \return status
    // template <typename T>
    // EasySerializeStatus to_json_file(const std::string &filename, T &obj,
    //                                  JsonIndent json_indent = JsonIndent::two_spaces)
    // {
    //     rapidjson::StringBuffer string_buffer;
    //     rapidjson_impl::to_json_buffer(string_buffer, obj, json_indent);
    //     EasySerializeStatus status;
    //     File *fp = fopen(filename.c_str());
    //     // TODO: file handling stuff
    //     fclose(fp);
    //     return status;
    // }

    // // Write vector of objects UTF-8 JSON to file.
    // //
    // // \param filename: "path/to/filename.json"
    // // \param v: vector of objects to write
    // // \param json_indent: JSON indent formatting
    // // \return status
    // template <typename T>
    // EasySerializeStatus to_json_file(const std::string &filename, std::vector<T> &v,
    //                                  JsonIndent json_indent = JsonIndent::two_spaces)
    // {
    // }

    // Write object UTF-8 JSON to a std::string.
    //
    // \param obj: object to write
    // \param json_indent: JSON indent formatting
    // \return object JSON in a std::string
    template <typename T>
    std::string to_json_string(T &obj,
                               JsonIndent json_indent = JsonIndent::two_spaces)
    {
        rapidjson::StringBuffer string_buffer;
        rapidjson_impl::to_json_buffer(string_buffer, obj, json_indent);
        return std::string(string_buffer.GetString(), string_buffer.GetSize());
    }

    // Write vector of objects UTF-8 JSON to std::string.
    //
    // \param v: vector of objects to write
    // \param json_indent: JSON indent formatting
    // \return vector of objects JSON in a std::string
    template <typename T>
    std::string to_json_string_vector_objects(std::vector<T> &v,
                                              JsonIndent json_indent = JsonIndent::two_spaces)
    {
        rapidjson::StringBuffer string_buffer;
        rapidjson_impl::to_json_buffer_vector_objects(string_buffer, v, json_indent);
        return std::string(string_buffer.GetString(), string_buffer.GetSize());
    }

    // Write vector of enums UTF-8 JSON to std::string.
    //
    // \param v: vector of enums to write
    // \param json_indent: JSON indent formatting
    // \return vector of objects JSON in a std::string
    template <typename T>
    std::string to_json_string_vector_enums(std::vector<T> &v,
                                            JsonIndent json_indent = JsonIndent::two_spaces)
    {
        rapidjson::StringBuffer string_buffer;
        rapidjson_impl::to_json_buffer_vector_enums(string_buffer, v, json_indent);
        return std::string(string_buffer.GetString(), string_buffer.GetSize());
    }

    // Write vector of supported C++ fundamental types UTF-8 JSON to std::string.
    //
    // \param v: vector of supported C++ fundamental types to write (including std::string)
    // \param json_indent: JSON indent formatting
    // \return vector of objects JSON in a std::string
    template <typename T>
    std::string to_json_string_vector(std::vector<T> &v,
                                      JsonIndent json_indent = JsonIndent::two_spaces)
    {
        rapidjson::StringBuffer string_buffer;
        rapidjson_impl::to_json_buffer_vector(string_buffer, v, json_indent);
        return std::string(string_buffer.GetString(), string_buffer.GetSize());
    }

} // namespace easy_serialize