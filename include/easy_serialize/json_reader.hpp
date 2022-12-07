#pragma once

#include "rapidjsonreader_impl.hpp"

#include <string>
#include <vector>

namespace easy_serialize
{
    // Populate object with UTF-8 json string in a buffer.
    //
    // \param buffer_ptr: pointer to char or uint8 array (gets reinterpret_cast to char*)
    // \param buffer_size: size of buffer.
    // \param obj: object to populate.
    // \return: EasySerializeStatus object. If bool operator() is true there were no errors.
    template <class BufferPtr, typename T>
    EasySerializeStatus from_json_buffer(BufferPtr buffer_ptr, size_t buffer_size, T &obj)
    {
        return rapidjson_impl::from_json_buffer(buffer_ptr, buffer_size, obj);
    }

    // Populate std::vector of objects with UTF-8 json string in buffer.
    //
    // \param buffer_ptr: pointer to char or uint8 array (gets reinterpret_cast to char*)
    // \param buffer_size: size of buffer.
    // \param v: vector of objects to populate.
    // \return: EasySerializeStatus object. If bool operator() is true there were no errors.
    // template <class BufferPtr, typename T>
    // EasySerializeStatus from_json_buffer_vector(BufferPtr buffer_ptr, size_t buffer_size, std::vector<T> &v)
    // {
    // }

    template <typename T>
    EasySerializeStatus from_json_string(const std::string &json_string, T &obj)
    {
        return from_json_buffer(json_string.c_str(), json_string.size(), obj);
    }

    // template <typename T>
    // EasySerializeStatus from_json_string(const std::string &json_string, std::vector<T> &obj)
    // {
    // }

} // namespace easy_serialize