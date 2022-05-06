#pragma once

#include "easy_serialize_status.hpp"

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/stream.h>

#include <cstdint>
#include <string>
#include <vector>

namespace easy_serialize
{
    namespace rapidjson_impl
    {
        // Json reader archive based on rapidjson.
        class RapidJsonReaderArchive
        {
        public:
            RapidJsonReaderArchive(std::istream &is);
            ~RapidJsonReaderArchive();

            void ez(const char *key, bool &);
            void ez(const char *key, int8_t &);
            void ez(const char *key, int16_t &);
            void ez(const char *key, int32_t &);
            void ez(const char *key, int64_t &);
            void ez(const char *key, uint8_t &);
            void ez(const char *key, uint16_t &);
            void ez(const char *key, uint32_t &);
            void ez(const char *key, uint64_t &);
            void ez(const char *key, float &);
            void ez(const char *key, double &);
            void ez(const char *key, std::string &);
            template <typename T>
            void ez_enum(const char *key, T &, T enum_value_N);
            template <typename T>
            void ez_object(const char *key, T &t);
            template <typename T>
            void ez_vector(const char *key, std::vector<T> &);

            RapidJsonReaderArchive(const RapidJsonReaderArchive &) = delete;
            RapidJsonReaderArchive &operator=(const RapidJsonReaderArchive &) = delete;

        private:
            void _ez(bool &);
            void _ez(int8_t &);
            void _ez(int16_t &);
            void _ez(int32_t &);
            void _ez(int64_t &);
            void _ez(uint8_t &);
            void _ez(uint16_t &);
            void _ez(uint32_t &);
            void _ez(uint64_t &);
            void _ez(float &);
            void _ez(double &);
            void _ez(std::string &);

            void checkKey(const char *key);

            rapidjson::IStreamWrapper _isw;
            std::vector<rapidjson::Value> _stack;
            rapidjson::Document _d;
        };

        template <class BufferPtr, typename T>
        EasySerializeStatus from_json_buffer(BufferPtr buffer_ptr, size_t buffer_size, T &obj)
        {
        }

        template <class BufferPtr, typename T>
        EasySerializeStatus from_json_buffer(BufferPtr buffer_ptr, size_t buffer_size, std::vector<T> &v)
        {
        }
    }

    //-------- for easy_serialize/json_reader.h -----------------------------
    // # include <string>
    // # include <vector>

    // Populate object with UTF-8 json string in a buffer.
    //
    // \param buffer_ptr: pointer to char or uint8 array (gets reinterpret_cast to char*)
    // \param buffer_size: size of buffer.
    // \param obj: object to populate.
    // \return: EasySerializeStatus object. If bool operator() is true there were no errors.
    template <class BufferPtr, typename T>
    EasySerializeStatus from_json_buffer(BufferPtr buffer_ptr, size_t buffer_size, T &obj)
    {
        // return rapidjson_impl::from_json_buffer(buffer_ptr, buffer_size, obj);
    }

    // Populate std::vector of objects with UTF-8 json string in buffer.
    //
    // \param buffer_ptr: pointer to char or uint8 array (gets reinterpret_cast to char*)
    // \param buffer_size: size of buffer.
    // \param v: vector of objects to populate.
    // \return: EasySerializeStatus object. If bool operator() is true there were no errors.
    template <class BufferPtr, typename T>
    EasySerializeStatus from_json_buffer(BufferPtr buffer_ptr, size_t buffer_size, std::vector<T> &v)
    {
    }

    // Populate object T
    template <typename T>
    EasySerializeStatus from_json_file(const std::string &filename, T &obj)
    {
    }

    // Populate object T
    template <typename T>
    EasySerializeStatus from_json_file(const std::string &filename, std::vector<T> &v)
    {
    }

    template <typename T>
    EasySerializeStatus from_json_string(const std::string &json_string, T &obj)
    {
    }

    template <typename T>
    EasySerializeStatus from_json_string(const std::string &json_string, std::vector<T> &obj)
    {
    }
}