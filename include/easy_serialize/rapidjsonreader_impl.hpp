// easy_serialize JSON reader implementation using rapidjson.
#pragma once

#include "easy_serialize_status.hpp"

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#include <cstdint>
#include <limits>
#include <string>
#include <vector>

namespace easy_serialize
{
    namespace rapidjson_impl
    {
        // JSON reader archive based on rapidjson.
        class RapidJsonReaderArchive
        {
        public:
            RapidJsonReaderArchive(){};
            void class_version(const int class_version_)
            {
                const auto it = _stack.back().value->FindMember("_objver");
                if (it != _stack.back().value->MemberEnd())
                {
                    try
                    {
                        _ez(it->value, _stack.back().objver);
                    }
                    catch (const std::exception &ex)
                    {
                        throw std::runtime_error(buildErrorKey("_objver") + ex.what());
                    }
                    if (class_version_ < _stack.back().objver)
                    {
                        throw std::runtime_error(buildErrorKey("_objver") + " object too new");
                    }
                }
                // else leave objver at zero default.
            }
            template <typename T>
            void ez(const char *key, T &t)
            {
                try
                {
                    const auto it = checkKey(key);
                    _ez(it->value, t);
                }
                catch (const std::exception &ex)
                {
                    throw std::runtime_error(buildErrorKey(key) + ex.what());
                }
            }
            template <typename T>
            void ez(const char *key, T &t, int object_version_supported)
            {
                if (object_version_supported > _stack.back().objver)
                {
                    return;
                }
                ez(key, t);
            }
            template <typename T>
            void ez_enum(const char *key, T &e, T enum_value_N)
            {
                try
                {
                    _ez_enum(checkKey(key)->value, e, enum_value_N);
                }
                catch (const std::exception &ex)
                {
                    throw std::runtime_error(buildErrorKey(key) + ex.what());
                }
            }
            template <typename T>
            void ez_enum(const char *key, T &e, T enum_value_N, int object_version_supported)
            {
                if (object_version_supported > _stack.back().objver)
                {
                    return;
                }
                ez_enum(key, e, enum_value_N);
            }
            template <typename T>
            void ez_object(const char *key, T &o)
            {
                try
                {
                    const auto it = checkKey(key);
                    _ez_object(it->value, o);
                }
                catch (const std::exception &ex)
                {
                    throw std::runtime_error(buildErrorKey(key) + ex.what());
                }
            }
            template <typename T>
            void ez_object(const char *key, T &o, int object_version_supported)
            {
                if (object_version_supported > _stack.back().objver)
                {
                    return;
                }
                ez_object(key, o);
            }
            template <typename T>
            void ez_vector(const char *key, std::vector<T> &v)
            {
                try
                {
                    _ez_vector(checkKey(key)->value, v);
                }
                catch (const std::exception &ex)
                {
                    throw std::runtime_error(buildErrorKey(key) + ex.what());
                }
            }
            template <typename T>
            void ez_vector(const char *key, std::vector<T> &v, int object_version_supported)
            {
                if (object_version_supported > _stack.back().objver)
                {
                    return;
                }
                ez_vector(key, v);
            }
            template <typename T>
            void ez_vector_enums(const char *key, std::vector<T> &v, T enum_value_N)
            {
                try
                {
                    _ez_vector_enums(checkKey(key)->value, v, enum_value_N);
                }
                catch (const std::exception &ex)
                {
                    throw std::runtime_error(buildErrorKey(key) + ex.what());
                }
            }
            template <typename T>
            void ez_vector_enums(const char *key, std::vector<T> &v, T enum_value_N, int object_version_supported)
            {
                if (object_version_supported > _stack.back().objver)
                {
                    return;
                }
                ez_vector_enums(key, v, enum_value_N);
            }
            template <typename T>
            void ez_vector_objects(const char *key, std::vector<T> &v)
            {
                try
                {
                    _ez_vector_objects(checkKey(key)->value, v);
                }
                catch (const std::exception &ex)
                {
                    throw std::runtime_error(buildErrorKey(key) + ex.what());
                }
            }
            template <typename T>
            void ez_vector_objects(const char *key, std::vector<T> &v, int object_version_supported)
            {
                if (object_version_supported > _stack.back().objver)
                {
                    return;
                }
                ez_vector_objects(key, v);
            }
            RapidJsonReaderArchive(const RapidJsonReaderArchive &) = delete;
            RapidJsonReaderArchive &operator=(const RapidJsonReaderArchive &) = delete;

            template <typename BufferPtr, typename T>
            friend EasySerializeStatus from_json_buffer(BufferPtr buffer_ptr, size_t buffer_size, T &obj);

            template <class BufferPtr, typename T>
            friend EasySerializeStatus from_json_buffer_vector_objects(BufferPtr buffer_ptr,
                                                                       size_t buffer_size,
                                                                       std::vector<T> &v);
            template <class BufferPtr, typename T>
            friend EasySerializeStatus from_json_buffer_vector(BufferPtr buffer_ptr,
                                                               size_t buffer_size,
                                                               std::vector<T> &v);
            template <typename BufferPtr, typename T>
            friend EasySerializeStatus from_json_buffer_vector_enums(BufferPtr buffer_ptr,
                                                                     size_t buffer_size,
                                                                     std::vector<T> &v,
                                                                     T enum_value_N);

        private:
            void _ez(const rapidjson::Value &value, bool &b)
            {
                if (!value.IsBool())
                {
                    throw std::runtime_error(" expected a bool");
                }
                b = value.GetBool();
            }
            void _ez(const rapidjson::Value &value, int8_t &i8)
            {
                if (!value.IsInt() ||
                    value.GetInt() < std::numeric_limits<int8_t>::min() ||
                    value.GetInt() > std::numeric_limits<int8_t>::max())
                {
                    throw std::runtime_error(" expected an int8");
                }
                i8 = static_cast<int8_t>(value.GetInt());
            }
            void _ez(const rapidjson::Value &value, int16_t &i16)
            {
                if (!value.IsInt() ||
                    value.GetInt() < std::numeric_limits<int16_t>::min() ||
                    value.GetInt() > std::numeric_limits<int16_t>::max())
                {
                    throw std::runtime_error(" expected an int16");
                }
                i16 = static_cast<int16_t>(value.GetInt());
            }
            void _ez(const rapidjson::Value &value, int32_t &i32)
            {
                if (!value.IsInt())
                {
                    throw std::runtime_error(" expected an int32");
                }
                i32 = value.GetInt();
            }
            void _ez(const rapidjson::Value &value, int64_t &i64)
            {
                if (!value.IsInt64())
                {
                    throw std::runtime_error(" expected an int64");
                }
                i64 = value.GetInt64();
            }
            void _ez(const rapidjson::Value &value, uint8_t &u8)
            {
                if (!value.IsUint() ||
                    value.GetUint() > std::numeric_limits<uint8_t>::max())
                {
                    throw std::runtime_error(" expected a uint8");
                }
                u8 = static_cast<uint8_t>(value.GetUint());
            }
            void _ez(const rapidjson::Value &value, uint16_t &u16)
            {
                if (!value.IsUint() ||
                    value.GetUint() > std::numeric_limits<uint16_t>::max())
                {
                    throw std::runtime_error(" expected a uint16");
                }
                u16 = static_cast<uint16_t>(value.GetUint());
            }
            void _ez(const rapidjson::Value &value, uint32_t &u32)
            {
                if (!value.IsUint())
                {
                    throw std::runtime_error(" expected a uint32");
                }
                u32 = value.GetUint();
            }
            void _ez(const rapidjson::Value &value, uint64_t &u64)
            {
                if (!value.IsUint64())
                {
                    throw std::runtime_error(" expected a uint64");
                }
                u64 = value.GetUint64();
            }
            // // Rapidjson doesn't support "float" (only "double"), so leave this out.
            // void _ez(const rapidjson::Value &value, float &f)
            void _ez(const rapidjson::Value &value, double &d)
            {
                if (!value.IsDouble())
                {
                    throw std::runtime_error(" expected a double");
                }
                d = value.GetDouble();
            }
            void _ez(const rapidjson::Value &value, std::string &s)
            {
                if (!value.IsString())
                {
                    throw std::runtime_error(" expected a string");
                }
                s = value.GetString();
            }
            template <typename T>
            void _ez_object(const rapidjson::Value &value, T &obj)
            {
                if (!value.IsObject())
                {
                    throw std::runtime_error(" expected an object");
                }
                _stack.emplace_back(&value, 0);
                obj.serialize(*this);
                _stack.pop_back();
            }
            template <typename T>
            void _ez_enum(const rapidjson::Value &value, T &e, T enum_value_N)
            {
                std::string s;
                _ez(value, s);
                for (int i = 0; i < static_cast<int>(enum_value_N); ++i)
                {
                    T t = static_cast<T>(i);
                    if (s == to_string(t))
                    {
                        e = t;
                        return;
                    }
                }
                throw std::runtime_error(" expected an enum type");
            }
            template <typename T>
            void _ez_vector(const rapidjson::Value &value, std::vector<T> &v)
            {
                if (!value.IsArray())
                {
                    throw std::runtime_error(" expected an array");
                }
                v.clear();
                v.reserve(value.Size());
                for (rapidjson::SizeType i = 0; i < value.Size(); ++i)
                {
                    try
                    {
                        T t;
                        _ez(value[i], t);
                        v.push_back(t);
                    }
                    catch (const std::exception &ex)
                    {
                        throw std::runtime_error(buildErrorIndex(i) + ex.what());
                    }
                }
            }
            template <typename T>
            void _ez_vector_objects(const rapidjson::Value &value, std::vector<T> &v)
            {
                if (!value.IsArray())
                {
                    throw std::runtime_error(" expected an array");
                }
                v.clear();
                v.reserve(value.Size());
                for (rapidjson::SizeType i = 0; i < value.Size(); ++i)
                {
                    try
                    {
                        T object;
                        _ez_object(value[i], object);
                        v.push_back(std::move(object));
                    }
                    catch (const std::exception &ex)
                    {
                        throw std::runtime_error(buildErrorIndex(i) + ex.what());
                    }
                }
            }
            template <typename T>
            void _ez_vector_enums(const rapidjson::Value &value, std::vector<T> &v, T enum_value_N)
            {
                if (!value.IsArray())
                {
                    throw std::runtime_error(" expected an array");
                }
                v.clear();
                v.reserve(value.Size());
                for (rapidjson::SizeType i = 0; i < value.Size(); ++i)
                {
                    try
                    {
                        T t;
                        _ez_enum(value[i], t, enum_value_N);
                        v.push_back(t);
                    }
                    catch (const std::exception &ex)
                    {
                        throw std::runtime_error(buildErrorIndex(i) + ex.what());
                    }
                }
            }
            rapidjson::Value::ConstMemberIterator checkKey(const char *key)
            {
                const auto it = _stack.back().value->FindMember(key);
                if (it == _stack.back().value->MemberEnd())
                {
                    throw std::runtime_error(" key not found");
                }
                return it;
            }
            std::string buildErrorKey(const char *key)
            {
                return std::string("[\"") + key + "\"]";
            }
            std::string buildErrorIndex(unsigned key)
            {
                return std::string("[") + std::to_string(key) + "]";
            }
            struct ValueObjVer
            {
                ValueObjVer(const rapidjson::Value *value_, int objver_) : value(value_), objver(objver_) {}
                const rapidjson::Value *value;
                int objver;
            };
            std::vector<ValueObjVer> _stack;
        };

        constexpr int RAPIDJSON_PARSE_FLAGS = rapidjson::kParseValidateEncodingFlag | rapidjson::kParseNanAndInfFlag | rapidjson::kParseFullPrecisionFlag;

        // Populate object with UTF-8 JSON in a buffer.
        //
        // \param buffer_ptr: pointer to buffer of UTF-8 JSON (gets reinterpret_cast to char*)
        // \param buffer_size: size of buffer
        // \param obj: object to populate
        // \return: EasySerializeStatus object
        template <typename BufferPtr, typename T>
        EasySerializeStatus from_json_buffer(BufferPtr buffer_ptr, size_t buffer_size, T &obj)
        {
            EasySerializeStatus status;
            rapidjson::Document _d;
            _d.Parse<RAPIDJSON_PARSE_FLAGS>(buffer_ptr, buffer_size);
            if (_d.HasParseError())
            {
                status.set_error_message(rapidjson::GetParseError_En(_d.GetParseError()));
                return status;
            }
            try
            {
                RapidJsonReaderArchive a;
                a._ez_object(_d, obj);
            }
            catch (const std::exception &ex)
            {
                status.set_error_message(ex.what());
            }
            return status;
        }

        // Populate std::vector of objects with UTF-8 JSON in buffer.
        //
        // \param buffer_ptr: pointer to buffer of UTF-8 JSON (gets reinterpret_cast to char*)
        // \param buffer_size: size of buffer
        // \param v: vector of objects to populate
        // \return: EasySerializeStatus object
        template <class BufferPtr, typename T>
        EasySerializeStatus from_json_buffer_vector_objects(BufferPtr buffer_ptr,
                                                            size_t buffer_size,
                                                            std::vector<T> &v)
        {
            EasySerializeStatus status;
            rapidjson::Document _d;
            _d.Parse<RAPIDJSON_PARSE_FLAGS>(buffer_ptr, buffer_size);
            if (_d.HasParseError())
            {
                status.set_error_message(rapidjson::GetParseError_En(_d.GetParseError()));
                return status;
            }
            try
            {
                RapidJsonReaderArchive a;
                a._ez_vector_objects(_d, v);
            }
            catch (const std::exception &ex)
            {
                status.set_error_message(ex.what());
            }
            return status;
        }

        // Populate std::vector of primitive types with UTF-8 JSON in buffer.
        //
        // \param buffer_ptr: pointer to buffer of UTF-8 JSON (gets reinterpret_cast to char*)
        // \param buffer_size: size of buffer
        // \param v: vector of primitive types to populate
        // \return: EasySerializeStatus object
        template <typename BufferPtr, typename T>
        EasySerializeStatus from_json_buffer_vector(BufferPtr buffer_ptr, size_t buffer_size, std::vector<T> &v)
        {
            EasySerializeStatus status;
            rapidjson::Document _d;
            _d.Parse<RAPIDJSON_PARSE_FLAGS>(buffer_ptr, buffer_size);
            if (_d.HasParseError())
            {
                status.set_error_message(rapidjson::GetParseError_En(_d.GetParseError()));
                return status;
            }
            try
            {
                RapidJsonReaderArchive a;
                a._ez_vector(_d, v);
            }
            catch (const std::exception &ex)
            {
                status.set_error_message(ex.what());
            }
            return status;
        }

        // Populate std::vector of enums with UTF-8 JSON in buffer.
        //
        // Constraints:
        //  * The client must define a char* to_string(Enum) function. The returned strings
        //    must be unique.
        //  * The enum integer values must be contiguous from  0 to < enum_value_N.
        //
        // \param buffer_ptr: pointer to buffer of UTF-8 JSON (gets reinterpret_cast to char*)
        // \param buffer_size: size of buffer
        // \param v: vector of enums to populate
        // \param enum_value_N: Last enum value (not a valid enum)
        // \return: EasySerializeStatus object
        template <typename BufferPtr, typename T>
        EasySerializeStatus from_json_buffer_vector_enums(BufferPtr buffer_ptr, size_t buffer_size,
                                                          std::vector<T> &v, T enum_value_N)
        {
            EasySerializeStatus status;
            rapidjson::Document _d;
            _d.Parse<RAPIDJSON_PARSE_FLAGS>(buffer_ptr, buffer_size);
            if (_d.HasParseError())
            {
                status.set_error_message(rapidjson::GetParseError_En(_d.GetParseError()));
                return status;
            }
            try
            {
                RapidJsonReaderArchive a;
                a._ez_vector_enums(_d, v, enum_value_N);
            }
            catch (const std::exception &ex)
            {
                status.set_error_message(ex.what());
            }
            return status;
        }
    }
}
