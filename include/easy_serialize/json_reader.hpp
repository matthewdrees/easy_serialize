// easy_serialize JSON reader implementation.
#pragma once

#include "rapidjsonreader_impl.hpp"

#include <string>
#include <vector>

namespace easy_serialize
{
    // Populate an object from a UTF-8 JSON string.
    //
    // \param json: std::string of UTF-8 JSON
    // \param obj: object to populate
    // \return: EasySerializeStatus object
    template <typename T>
    EasySerializeStatus from_json_string(const std::string &json, T &obj)
    {
        return rapidjson_impl::from_json_buffer(json.data(), json.size(), obj);
    }

    // Populate a std::vector of objects from a UTF-8 JSON string.
    //
    // \param json: std::string of UTF-8 JSON
    // \param v: vector of objects to populate
    // \return: EasySerializeStatus object
    template <typename T>
    EasySerializeStatus from_json_string_vector_objects(const std::string &json, std::vector<T> &v)
    {
        return rapidjson_impl::from_json_buffer_vector_objects(json.data(), json.size(), v);
    }

    // Populate a std::vector of primitive types from a UTF-8 JSON string.
    //
    // \param json: std::string of UTF-8 JSON
    // \param v: vector of primitive types to populate
    // \return: EasySerializeStatus object
    template <typename T>
    EasySerializeStatus from_json_string_vector(const std::string &json, std::vector<T> &v)
    {
        return rapidjson_impl::from_json_buffer_vector(json.data(), json.size(), v);
    }

    // Populate a std::vector of enums from a UTF-8 JSON string.
    //
    // Constraints:
    //  * The client must define a char* to_string(Enum) function. The returned strings
    //    must be unique.
    //  * The enum integer values must be contiguous from  0 to < enum_value_N.
    //
    // \param json: std::string of UTF-8 JSON
    // \param v: vector of enums to populate
    // \param enum_value_N: last enum value (not a valid value)
    // \return: EasySerializeStatus object
    template <typename T>
    EasySerializeStatus from_json_string_vector_enums(const std::string &json, std::vector<T> &v,
                                                      T enum_value_N)
    {
        return rapidjson_impl::from_json_buffer_vector_enums(json.data(), json.size(), v, enum_value_N);
    }
} // namespace easy_serialize
