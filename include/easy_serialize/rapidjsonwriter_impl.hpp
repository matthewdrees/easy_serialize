#pragma once

#include "easy_serialize_status.hpp"
#include "json_indent.hpp"

#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

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
            explicit RapidJsonWriterArchive(rapidjson::PrettyWriter<rapidjson::StringBuffer> &writer_) : _writer(writer_) {}
            void class_version(const int class_version_)
            {
                if (class_version_ > 0)
                {
                    _writer.Key("_objver");
                    _writer.Int(class_version_);
                }
            }
            template <typename T>
            void ez(const char *key, T t, int /*object_version_supported*/)
            {
                ez(key, t);
            }
            void ez(const char *key, bool b)
            {
                _writer.Key(key);
                _ez(b);
            }
            void ez(const char *key, int8_t i8)
            {
                _writer.Key(key);
                _ez(i8);
            }
            void ez(const char *key, int16_t i16)
            {
                _writer.Key(key);
                _ez(i16);
            }
            void ez(const char *key, int32_t i32)
            {
                _writer.Key(key);
                _ez(i32);
            }
            void ez(const char *key, int64_t i64)
            {
                _writer.Key(key);
                _ez(i64);
            }
            void ez(const char *key, uint8_t u8)
            {
                _writer.Key(key);
                _ez(u8);
            }
            void ez(const char *key, uint16_t u16)
            {
                _writer.Key(key);
                _ez(u16);
            }
            void ez(const char *key, uint32_t u32)
            {
                _writer.Key(key);
                _ez(u32);
            }
            void ez(const char *key, uint64_t u64)
            {
                _writer.Key(key);
                _ez(u64);
            }
            // Rapidjson doesn't support "float" (only "double"), so leave this out.
            // void ez(const char *key, float f)
            void ez(const char *key, double d)
            {
                _writer.Key(key);
                _ez(d);
            }
            void ez(const char *key, const std::string &s)
            {
                _writer.Key(key);
                _ez(s);
            }
            template <typename T>
            void ez_enum(const char *key, T e, T /* enum_value_N */)
            {
                _writer.Key(key);
                _ez_enum(e);
            }
            template <typename T>
            void ez_enum(const char *key, T e, T enum_value_N, int /*object_version_supported*/)
            {
                ez_enum(key, e, enum_value_N);
            }
            template <typename T>
            void ez_object(const char *key, T &o)
            {
                _writer.Key(key);
                _ez_object(o);
            }
            template <typename T>
            void ez_object(const char *key, T t, int /*object_version_supported*/)
            {
                ez_object(key, t);
            }
            template <typename T>
            void ez_vector(const char *key, std::vector<T> &v)
            {
                _writer.Key(key);
                _ez_vector(v);
            }
            template <typename T>
            void ez_vector(const char *key, std::vector<T> v, int /*object_version_supported*/)
            {
                ez_vector(key, v);
            }
            template <typename T>
            void ez_vector_enums(const char *key, std::vector<T> &v, T /*enum_value_N*/)
            {
                _writer.Key(key);
                _ez_vector_enums(v);
            }
            template <typename T>
            void ez_vector_enums(const char *key, std::vector<T> &v, T enum_value_N, int /*object_version_supported*/)
            {
                ez_vector_enums(key, v, enum_value_N);
            }
            template <typename T>
            void ez_vector_objects(const char *key, std::vector<T> &v)
            {
                _writer.Key(key);
                _ez_vector_objects(v);
            }
            template <typename T>
            void ez_vector_objects(const char *key, std::vector<T> v, int /*object_version_supported*/)
            {
                ez_vector_objects(key, v);
            }

            RapidJsonWriterArchive(const RapidJsonWriterArchive &) = delete;
            RapidJsonWriterArchive &operator=(const RapidJsonWriterArchive &) = delete;

            template <typename T>
            friend void to_json_buffer(rapidjson::StringBuffer &string_buffer, T &obj,
                                       JsonIndent json_indent);
            template <typename T>
            friend void to_json_buffer_vector_objects(rapidjson::StringBuffer &string_buffer, std::vector<T> &v,
                                                      JsonIndent json_indent);
            template <typename T>
            friend void to_json_buffer_vector_enums(rapidjson::StringBuffer &string_buffer, std::vector<T> &v,
                                                    JsonIndent json_indent);
            template <typename T>
            friend void to_json_buffer_vector(rapidjson::StringBuffer &string_buffer, std::vector<T> &v,
                                              JsonIndent json_indent);

        private:
            void _ez(bool b)
            {
                _writer.Bool(b);
            }
            void _ez(int8_t i8)
            {
                _writer.Int(i8);
            }
            void _ez(int16_t i16)
            {
                _writer.Int(i16);
            }
            void _ez(int32_t i32)
            {
                _writer.Int(i32);
            }
            void _ez(int64_t i64)
            {
                _writer.Int64(i64);
            }
            void _ez(uint8_t u8)
            {
                _writer.Uint(u8);
            }
            void _ez(uint16_t u16)
            {
                _writer.Uint(u16);
            }
            void _ez(uint32_t u32)
            {
                _writer.Uint(u32);
            }
            void _ez(uint64_t u64)
            {
                _writer.Uint64(u64);
            }
            void _ez(double d)
            {
                _writer.Double(d);
            }
            void _ez(const std::string &s)
            {
                _writer.String(s);
            }
            template <typename T>
            void _ez_enum(T &e)
            {
                _writer.String(to_string(e));
            }
            template <typename T>
            void _ez_object(T &o)
            {
                _writer.StartObject();
                o.serialize(*this);
                _writer.EndObject();
            }
            template <typename T>
            void _ez_vector(std::vector<T> &v)
            {
                _writer.StartArray();
                for (const auto &t : v)
                {
                    _ez(t);
                }
                _writer.EndArray();
            }
            template <typename T>
            void _ez_vector_enums(std::vector<T> &v)
            {
                _writer.StartArray();
                for (const auto e : v)
                {
                    _ez_enum(e);
                }
                _writer.EndArray();
            }
            template <typename T>
            void _ez_vector_objects(std::vector<T> &v)
            {
                _writer.StartArray();
                for (auto &o : v)
                {
                    _writer.StartObject();
                    o.serialize(*this);
                    _writer.EndObject();
                }
                _writer.EndArray();
            }
            rapidjson::PrettyWriter<rapidjson::StringBuffer> &_writer;
        };

        // Create UTF-8 JSON from object in memory buffer.
        //
        // \param string_buffer: rapidjson StringBuffer output stream buffer.
        // \param obj: object to json
        // \param json_indent: json indent formatting
        template <typename T>
        void to_json_buffer(rapidjson::StringBuffer &string_buffer, T &obj,
                            JsonIndent json_indent)
        {
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(string_buffer);
            RapidJsonWriterArchive a(writer);
            writer.SetIndent(' ', get_num_spaces(json_indent));
            a._ez_object(obj);
        }

        // Create UTF-8 JSON from vector of objects in memory buffer.
        //
        // \param string_buffer: rapidjson::StringBuffer
        // \param v: vector of objects
        // \param num_indent_spaces: Number of spaces to indent. If 0, make compact (no newlines).
        template <typename T>
        void to_json_buffer_vector_objects(rapidjson::StringBuffer &string_buffer, std::vector<T> &v,
                                           JsonIndent json_indent)
        {
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(string_buffer);
            writer.SetIndent(' ', get_num_spaces(json_indent));
            RapidJsonWriterArchive a(writer);
            a._ez_vector_objects(v);
        }

        // Create UTF-8 JSON from vector of supported fundamental types in memory buffer.
        //
        // \param string_buffer: rapidjson::StringBuffer
        // \param obj: object to json
        // \param num_indent_spaces: Number of spaces to indent. If 0, make compact (no newlines).
        template <typename T>
        void to_json_buffer_vector(rapidjson::StringBuffer &string_buffer, std::vector<T> &v,
                                   JsonIndent json_indent)
        {
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(string_buffer);
            writer.SetIndent(' ', get_num_spaces(json_indent));
            RapidJsonWriterArchive a(writer);
            a._ez_vector(v);
        }

        // Create UTF-8 JSON from vector of enums in memory buffer.
        //
        // \param string_buffer: rapidjson::StringBuffer
        // \param obj: object to json
        // \param num_indent_spaces: Number of spaces to indent. If 0, make compact (no newlines).
        template <typename T>
        void to_json_buffer_vector_enums(rapidjson::StringBuffer &string_buffer, std::vector<T> &v,
                                         JsonIndent json_indent)
        {
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(string_buffer);
            writer.SetIndent(' ', get_num_spaces(json_indent));
            RapidJsonWriterArchive a(writer);
            a._ez_vector_enums(v);
        }
    } // namespace rapidjson_impl
} // namespace easy_serialize
