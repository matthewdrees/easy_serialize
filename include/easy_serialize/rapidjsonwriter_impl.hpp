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

            void class_version(int class_version)
            {
                _writer.Key("_objver");
                _writer.Int(class_version);
            }
            void object_version_supported(int /* supported_object_version */)
            {
                // Writers don't care.
            }
            void ez(const char *key, bool b)
            {
                _writer.Key(key);
                _ez(b);
            };
            void ez(const char *key, int8_t i)
            {
                _writer.Key(key);
                _ez(i);
            }
            void ez(const char *key, int16_t i)
            {
                _writer.Key(key);
                _ez(i);
            }
            void ez(const char *key, int32_t i)
            {
                _writer.Key(key);
                _ez(i);
            }
            void ez(const char *key, int64_t i)
            {
                _writer.Key(key);
                _ez(i);
            }
            void ez(const char *key, uint8_t u)
            {
                _writer.Key(key);
                _ez(u);
            }
            void ez(const char *key, uint16_t u)
            {
                _writer.Key(key);
                _ez(u);
            }
            void ez(const char *key, uint32_t u)
            {
                _writer.Key(key);
                _ez(u);
            }
            void ez(const char *key, uint64_t u)
            {
                _writer.Key(key);
                _ez(u);
            }

            // Rapidjson doesn't support "float" (only "double"), so leave this out.
            // void ez(const char *key, float f)

            void ez(const char *key, double d)
            {
                _writer.Key(key);
                _ez(d);
            }
            void ez(const char *key, std::string &s)
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
            void ez_object(const char *key, T &o)
            {
                _writer.Key(key);
                _ez_object(o);
            }
            template <typename T>
            void ez_vector_objects(const char *key, std::vector<T> &v)
            {
                _writer.Key(key);
                _ez_vector_objects(v);
            }
            template <typename T>
            void ez_vector_enums(const char *key, std::vector<T> &v)
            {
                _writer.Key(key);
                _ez_vector_enums(v);
            }
            template <typename T>
            void ez_vector(const char *key, std::vector<T> &v)
            {
                _writer.Key(key);
                _ez_vector(v);
            }

            RapidJsonWriterArchive(const RapidJsonWriterArchive &) = delete;
            RapidJsonWriterArchive &operator=(const RapidJsonWriterArchive &) = delete;

            template <typename T>
            void _ez_object(T &o)
            {
                _writer.StartObject();
                o.serialize(*this);
                _writer.EndObject();
            }
            template <typename T>
            void _ez_enum(T &e)
            {
                _writer.String(to_string(e));
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
            template <typename T>
            void _ez_vector_enums(std::vector<T> &v)
            {
                _writer.StartArray();
                for (auto &e : v)
                {
                    _ez_enum(e);
                }
                _writer.EndArray();
            }
            template <typename T>
            void _ez_vector(std::vector<T> &v)
            {
                _writer.StartArray();
                for (auto &t : v)
                {
                    _ez(t);
                }
                _writer.EndArray();
            }

        private:
            void _ez(bool b)
            {
                _writer.Bool(b);
            }
            void _ez(int8_t i)
            {
                _writer.Int(i);
            }
            void _ez(int16_t i)
            {
                _writer.Int(i);
            }
            void _ez(int32_t i)
            {
                _writer.Int(i);
            }
            void _ez(int64_t i)
            {
                _writer.Int64(i);
            }
            void _ez(uint8_t u)
            {
                _writer.Uint(u);
            }
            void _ez(uint16_t u)
            {
                _writer.Uint(u);
            }
            void _ez(uint32_t u)
            {
                _writer.Uint(u);
            }
            void _ez(uint64_t u)
            {
                _writer.Uint64(u);
            }
            void _ez(double d)
            {
                _writer.Double(d);
            }
            void _ez(const std::string &s)
            {
                _writer.String(s);
            }

            rapidjson::PrettyWriter<rapidjson::StringBuffer> &_writer;
        };

        inline unsigned get_num_spaces(JsonIndent json_indent)
        {
            switch (json_indent)
            {
            case JsonIndent::compact:
                break;
            case JsonIndent::one_space:
                return 1;
            case JsonIndent::two_spaces:
                return 2;
            case JsonIndent::three_spaces:
                return 3;
            case JsonIndent::four_spaces:
                return 4;
            }
            return 0;
        }

        // Create UTF-8 JSON from object in memory buffer.
        //
        // \param string_buffer: rapidjson StringBuffer output stream buffer.
        // \param obj: object to json
        // \param json_indent: json indent formatting
        template <typename T>
        void to_json_buffer(rapidjson::StringBuffer &string_buffer, T &obj,
                            JsonIndent json_indent = JsonIndent::two_spaces)
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
                                           JsonIndent json_indent = JsonIndent::two_spaces)
        {
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(string_buffer);
            writer.SetIndent(' ', get_num_spaces(json_indent));
            RapidJsonWriterArchive a(writer);
            a._ez_vector_objects(v);
        }

        // Create UTF-8 JSON from vector of enums in memory buffer.
        //
        // \param string_buffer: rapidjson::StringBuffer
        // \param obj: object to json
        // \param num_indent_spaces: Number of spaces to indent. If 0, make compact (no newlines).
        template <typename T>
        void to_json_buffer_vector_enums(rapidjson::StringBuffer &string_buffer, std::vector<T> &v,
                                         JsonIndent json_indent = JsonIndent::two_spaces)
        {
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(string_buffer);
            writer.SetIndent(' ', get_num_spaces(json_indent));
            RapidJsonWriterArchive a(writer);
            a._ez_vector_enums(v);
        }

        // Create UTF-8 JSON from vector of supported fundamental types in memory buffer.
        //
        // \param string_buffer: rapidjson::StringBuffer
        // \param obj: object to json
        // \param num_indent_spaces: Number of spaces to indent. If 0, make compact (no newlines).
        template <typename T>
        void to_json_buffer_vector(rapidjson::StringBuffer &string_buffer, std::vector<T> &v,
                                   JsonIndent json_indent = JsonIndent::two_spaces)
        {
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(string_buffer);
            writer.SetIndent(' ', get_num_spaces(json_indent));
            RapidJsonWriterArchive a(writer);
            a._ez_vector(v);
        }

    } // namespace rapidjson_impl
} // namespace easy_serialize
