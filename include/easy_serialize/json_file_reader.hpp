// easy_serialize JSON helper functions for reading from a file.
#pragma once

#include "json_reader.hpp"
#include <cstdio>

namespace easy_serialize
{
    namespace // unnamed
    {
        // Helper function for reading json from a file.
        EasySerializeStatus from_file(const std::string &filename, std::string &json)
        {
            EasySerializeStatus status;
            std::FILE *fp = fopen(filename.c_str(), "r");
            if (!fp)
            {
                status.set_error_message("File opening failed.");
            }
            else
            {
                fseek(fp, 0L, SEEK_END);
                const auto sz = ftell(fp);
                if (sz < 0)
                {
                    status.set_error_message("File reading error. Too big?");
                }
                else
                {
                    std::rewind(fp);
                    json.resize(static_cast<size_t>(sz));
                    std::ignore = std::fread(&json[0], sizeof(json[0]), json.size(), fp);
                }
                fclose(fp);
            }
            return status;
        }
    } // unnamed namespace

    // Populate an object from a UTF-8 JSON file.
    //
    // \param filename: name of file
    // \param obj: object to populate
    // \return: EasySerializeStatus object
    template <typename T>
    EasySerializeStatus from_json_file(const std::string &filename, T &obj)
    {
        std::string json;
        EasySerializeStatus status = from_file(filename, json);
        if (!status)
        {
            return status;
        }
        return from_json_string(json, obj);
    }

    // Populate a vector of objects from a UTF-8 JSON file.
    //
    // \param filename: name of file
    // \param obj: object to populate
    // \return: EasySerializeStatus object
    template <typename T>
    EasySerializeStatus from_json_file_vector_objects(const std::string &filename, std::vector<T> &v)
    {
        std::string json;
        EasySerializeStatus status = from_file(filename, json);
        if (!status)
        {
            return status;
        }
        return from_json_string_vector_objects(json, v);
    }

    // Populate a vector of primitive types from a UTF-8 JSON file.
    //
    // \param filename: name of file
    // \param obj: vector to populate
    // \return: EasySerializeStatus object
    template <typename T>
    EasySerializeStatus from_json_file_vector(const std::string &filename, std::vector<T> &v)
    {
        std::string json;
        EasySerializeStatus status = from_file(filename, json);
        if (!status)
        {
            return status;
        }
        return from_json_string_vector(json, v);
    }

    // Populate a vector of enums from a UTF-8 JSON file.
    //
    // Constraints:
    //  * The client must define a char* to_string(Enum) function. The returned strings
    //    must be unique.
    //  * The enum integer values must be contiguous from  0 to < enum_value_N.
    //
    // \param filename: name of file
    // \param obj: vector to populate
    // \param enum_value_N: last enum value (not a valid value)
    // \return: EasySerializeStatus object
    template <typename T>
    EasySerializeStatus from_json_file_vector_enums(const std::string &filename, std::vector<T> &v,
                                                    T enum_value_N)
    {
        std::string json;
        EasySerializeStatus status = from_file(filename, json);
        if (!status)
        {
            return status;
        }
        return from_json_string_vector_enums(json, v, enum_value_N);
    }

} // namespace easy_serialize
