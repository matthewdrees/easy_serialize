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
        inline std::string buildErrorKey(const char *key)
        {
            return std::string("[\"") + key + "\"]";
        }

        // inline std::string buildErrorMessage(const char *key, const std::string &message)
        // {
        //     return buildErrorKey(key) + message;
        // }

        inline std::string buildErrorIndex(unsigned key)
        {
            return std::string("[") + std::to_string(key) + "]";
        }
        // Json reader archive based on rapidjson.
        class RapidJsonReaderArchive
        {
        public:
            RapidJsonReaderArchive(){};
            void ez(const char *key, bool &b)
            {
                try
                {
                    const auto it = checkKey(key);
                    _ez(it->value, b);
                }
                catch (const std::exception &ex)
                {
                    throw std::runtime_error(buildErrorKey(key) + ex.what());
                }
            }
            void ez(const char *key, int8_t &i)
            {
                try
                {
                    const auto it = checkKey(key);
                    _ez(it->value, i);
                }
                catch (const std::exception &ex)
                {
                    throw std::runtime_error(buildErrorKey(key) + ex.what());
                }
            }
            void ez(const char *key, int16_t &i)
            {
                try
                {
                    const auto it = checkKey(key);
                    _ez(it->value, i);
                }
                catch (const std::exception &ex)
                {
                    throw std::runtime_error(buildErrorKey(key) + ex.what());
                }
            }
            void ez(const char *key, int32_t &i)
            {
                try
                {
                    const auto it = checkKey(key);
                    _ez(it->value, i);
                }
                catch (const std::exception &ex)
                {
                    throw std::runtime_error(buildErrorKey(key) + ex.what());
                }
            }
            void ez(const char *key, int64_t &i)
            {
                try
                {
                    const auto it = checkKey(key);
                    _ez(it->value, i);
                }
                catch (const std::exception &ex)
                {
                    throw std::runtime_error(buildErrorKey(key) + ex.what());
                }
            }
            void ez(const char *key, uint8_t &u)
            {
                try
                {
                    const auto it = checkKey(key);
                    _ez(it->value, u);
                }
                catch (const std::exception &ex)
                {
                    throw std::runtime_error(buildErrorKey(key) + ex.what());
                }
            }
            void ez(const char *key, uint16_t &u)
            {
                try
                {
                    const auto it = checkKey(key);
                    _ez(it->value, u);
                }
                catch (const std::exception &ex)
                {
                    throw std::runtime_error(buildErrorKey(key) + ex.what());
                }
            }
            void ez(const char *key, uint32_t &u)
            {
                try
                {
                    const auto it = checkKey(key);
                    _ez(it->value, u);
                }
                catch (const std::exception &ex)
                {
                    throw std::runtime_error(buildErrorKey(key) + ex.what());
                }
            }
            void ez(const char *key, uint64_t &u)
            {
                try
                {
                    const auto it = checkKey(key);
                    _ez(it->value, u);
                }
                catch (const std::exception &ex)
                {
                    throw std::runtime_error(buildErrorKey(key) + ex.what());
                }
            }
            // void ez(const char *key, float &);
            void ez(const char *key, double &d)
            {
                try
                {
                    _ez(checkKey(key)->value, d);
                }
                catch (const std::exception &ex)
                {
                    throw std::runtime_error(buildErrorKey(key) + ex.what());
                }
            }
            void ez(const char *key, std::string &s)
            {
                try
                {
                    _ez(checkKey(key)->value, s);
                }
                catch (const std::exception &ex)
                {
                    throw std::runtime_error(buildErrorKey(key) + ex.what());
                }
            }
            template <typename T>
            void ez_enum(const char *key, T &e, T enum_value_N)
            {
                std::string s;
                ez(key, s);
                for (int i = 0; i < static_cast<int>(enum_value_N); ++i)
                {
                    T t = static_cast<T>(i);
                    if (s == to_string(t))
                    {
                        e = t;
                        return;
                    }
                }
                throw std::runtime_error(" bad enum value");
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
            LEFT OFF HERE template <typename T>
            void ez_vector(const char *key, std::vector<T> &v)
            try
            {
                const auto it = checkKey(key);
                const auto &value = it->value;
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
                        if (!value[i].IsObject())
                        {
                            throw std::runtime_error(" expected an object");
                        }
                        T o;
                        _stack.push_back(value[i].GetObject());
                        o.serialize(*this);
                        _stack.pop_back();
                        v_o.push_back(o);
                    }
                    catch (const std::exception &ex)
                    {
                        const std::string error = buildErrorIndex(i) + ex.what();
                        throw std::runtime_error(error);
                    }
                }
            }
            catch (const std::exception &ex)
            {
                const std::string error = buildErrorKey(key) + ex.what();
                throw std::runtime_error(error);
            }
        }

        RapidJsonReaderArchive(const RapidJsonReaderArchive &) = delete;
        RapidJsonReaderArchive &operator=(const RapidJsonReaderArchive &) = delete;

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
                throw std::runtime_error(" expected an int");
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
                throw std::runtime_error(" expected an int8");
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
        // void _ez(float &);
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

    public:
        template <typename T>
        void _ez_object(const rapidjson::Value &value, T &obj)
        {
            if (!value.IsObject())
            {
                throw std::runtime_error(" expected an object");
            }
            _stack.push_back(&value);
            // Function above this will catch exceptions.
            obj.serialize(*this);
            _stack.pop_back();
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
                }
                catch (const std::exception &ex)
                {
                    throw std::runtime_error(buildErrorIndex(i) + ex.what());
                }
            }
        }

    private:
        rapidjson::Value::ConstMemberIterator checkKey(const char *key)
        {
            const auto it = _stack.back()->FindMember(key);
            if (it == _stack.back()->MemberEnd())
            {
                throw std::runtime_error(" key doesn't exist");
            }
            return it;
        }

        std::vector<const rapidjson::Value *> _stack;
    };

    template <typename BufferPtr, typename T>
    EasySerializeStatus from_json_buffer(BufferPtr buffer_ptr, size_t buffer_size, T &obj)
    {
        EasySerializeStatus status;
        rapidjson::Document _d;
        _d.Parse<rapidjson::kParseValidateEncodingFlag | rapidjson::kParseNanAndInfFlag>(buffer_ptr, buffer_size);
        if (_d.HasParseError())
        {
            status.error_message = rapidjson::GetParseError_En(_d.GetParseError());
            return status;
        }
        try
        {
            RapidJsonReaderArchive a;
            a._ez_object(_d, obj);
        }
        catch (const std::exception &ex)
        {
            status.error_message = ex.what();
        }
        return status;
    }

    template <class BufferPtr, typename T>
    EasySerializeStatus from_json_buffer_vector_objects(BufferPtr buffer_ptr,
                                                        size_t buffer_size,
                                                        std::vector<T> &v)
    {
        EasySerializeStatus status;
        rapidjson::Document _d;
        _d.Parse<rapidjson::kParseValidateEncodingFlag | rapidjson::kParseNanAndInfFlag>(buffer_ptr, buffer_size);
        if (_d.HasParseError())
        {
            status.error_message = rapidjson::GetParseError_En(_d.GetParseError());
            return status;
        }
        try
        {
            RapidJsonReaderArchive a;
            a._ez_vector_objects(_d, v);
        }
        catch (const std::exception &ex)
        {
            status.error_message = ex.what();
        }
        return status;
    }
}
}