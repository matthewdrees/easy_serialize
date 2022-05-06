#pragma once

#include "easy_serialize_status.hpp"

#include <rapidjson/document.h>
#include <rapidjson/fwd.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

#include <cstdint>
#include <string>
#include <vector>

namespace easy_serialize
{
    namespace rapidjson_impl
    {
        // Json writer archive based on rapidjson.
        class RapidJsonWriterArchive
        {
        public:
            RapidJsonWriterArchive(std::ostream &, bool);
            ~RapidJsonWriterArchive();

            void version(int);
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

            RapidJsonWriterArchive(const RapidJsonWriterArchive &) = delete;
            RapidJsonWriterArchive &operator=(const RapidJsonWriterArchive &) = delete;

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

            const bool _is_object;
            rapidjson::OStreamWrapper _osw;
            rapidjson::PrettyWriter<rapidjson::OStreamWrapper> _writer;
        };

        // Create UTF-8 JSON from object in memory buffer.
        //
        // \param string_buffer: rapidjson StringBuffer output stream buffer.
        // \param obj: object to json
        // \param num_indent_spaces: Number of spaces to indent. If 0, make compact (no newlines).
        template <typename T>
        void to_json_buffer(rapidjson::StringBuffer &string_buffer, T &obj, int num_indent_spaces = 2)
        {
        }

        // Create UTF-8 JSON from vector of objects in memory buffer.
        //
        // \param string_buffer: rapidjson::StringBuffer
        // \param obj: object to json
        // \param num_indent_spaces: Number of spaces to indent. If 0, make compact (no newlines).
        template <typename T>
        void to_json_buffer(rapidjson::StringBuffer &string_buffer, std::vector<T> &v, int num_indent_spaces = 2)
        {
        }
    } // namespace rapidjson_impl

    //-------- for easy_serialize/json_writer.h -----------------------------
    // # include <string>
    // # include <vector>

    // Write UTF-8 JSON to file.
    //
    // \param filename: "path/to/filename.json"
    // \param obj: object to write
    // \param num_indent_spaces: Number of spaces to indent. If 0, make compact (no newlines).
    template <typename T>
    EasySerializeStatus to_json_file(const std::string &filename, T &obj, int num_indent_spaces = 2)
    {
        rapidjson::StringBuffer string_buffer;
        rapidjson_impl::to_json_buffer(string_buffer, obj, num_indent_spaces);
        EasySerializeStatus status;
        File *fp = fopen(filename.c_str());
        // TODO: file handling stuff
        fclose(fp);
        return status;
    }

    // Write object UTF-8 JSON to file, const ref version of function.
    //
    // const_cast away the const so we can use the non-const serialize object method. These functions don't
    // modify the object so no undefined behavior.
    template <typename T>
    EasySerializeStatus to_json_file(const std::string &filename, const T &obj, int num_indent_spaces = 2)
    {
        return to_json_file(filename, const_cast<T &>(obj), num_indent_spaces);
    }

    // Write vector of objects UTF-8 JSON to file.
    //
    template <typename T>
    EasySerializeStatus to_json_file(const std::string &filename, std::vector<T> &v, int num_indent_spaces = 2)
    {
    }
    template <typename T>
    EasySerializeStatus to_json_file(const std::string &filename, const std::vector<T> &v, int num_indent_spaces = 2)
    {
        return to_json_file(filename, const_cast<vector<T> &>(v), num_indent_spaces);
    }

    template <typename T>
    std::string to_json_string(T &obj, int num_indent_spaces = 2)
    {
        rapidjson::StringBuffer string_buffer;
        rapidjson_impl::to_json_buffer(string_buffer, obj, num_indent_spaces);
        return std::string(string_buffer.GetString(), string_buffer.GetSize());
    }
    template <typename T>
    std::string to_json_string(const T &obj, int num_indent_spaces = 2)
    {
        return to_json_string(const_cast<T &>(obj), num_indent_spaces);
    }

    template <typename T>
    std::string to_json_string(std::vector<T> &v, int num_indent_spaces = 2)
    {
        rapidjson::StringBuffer string_buffer;
        rapidjson_impl::to_json_buffer(string_buffer, v, num_indent_spaces);
        return std::string(string_buffer.GetString(), string_buffer.GetSize());
    }
    template <typename T>
    std::string to_json_string(const std::vector<T> &v, int num_indent_spaces = 2)
    {
        return to_json_string(filename, const_cast<vector<T> &>(v), num_indent_spaces);
    }

} // namespace easy_serialize
