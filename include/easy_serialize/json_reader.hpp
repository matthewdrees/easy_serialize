#pragma once

#include "rapidjsonreader_impl.hpp"

#include <string>
#include <vector>

namespace easy_serialize
{
    // Populate object with UTF-8 json in a buffer.
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

    // Populate std::vector of objects with UTF-8 json in buffer.
    //
    // \param buffer_ptr: pointer to char or uint8 array (gets reinterpret_cast to char*)
    // \param buffer_size: size of buffer.
    // \param v: vector of objects to populate.
    // \return: EasySerializeStatus object. If bool operator() is true there were no errors.
    template <class BufferPtr, typename T>
    EasySerializeStatus from_json_buffer_vector_objects(BufferPtr buffer_ptr, size_t buffer_size, std::vector<T> &v)
    {
        return rapidjson_impl::from_json_buffer_vector_objects(buffer_ptr, buffer_size, v);
    }

    // Populate std::vector of supported primitive types with UTF-8 json in buffer.
    //
    // \param buffer_ptr: pointer to char or uint8 array (gets reinterpret_cast to char*)
    // \param buffer_size: size of buffer.
    // \param v: vector of primitive types to populate.
    // \return: EasySerializeStatus object. If bool operator() is true there were no errors.
    template <class BufferPtr, typename T>
    EasySerializeStatus from_json_buffer_vector(BufferPtr buffer_ptr, size_t buffer_size, std::vector<T> &v)
    {
        return rapidjson_impl::from_json_buffer_vector(buffer_ptr, buffer_size, v);
    }

    // Populate std::vector of enums with UTF-8 json in buffer.
    //
    // \param buffer_ptr: pointer to char or uint8 array (gets reinterpret_cast to char*)
    // \param buffer_size: size of buffer.
    // \param v: vector of enums to populate.
    // \return: EasySerializeStatus object. If bool operator() is true there were no errors.
    template <class BufferPtr, typename T>
    EasySerializeStatus from_json_buffer_vector_enums(BufferPtr buffer_ptr, size_t buffer_size, std::vector<T> &v)
    {
        return rapidjson_impl::from_json_buffer_vector_enums(buffer_ptr, buffer_size, v);
    }
} // namespace easy_serialize
