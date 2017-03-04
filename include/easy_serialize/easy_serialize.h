// Copyright (c) 2016 Matt Drees, Fluke Networks
#pragma once

#include <exception>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "json_parser.h"

// easy_serialize is a library and a client usage pattern with the following goals:
//
//  1. Easy to use for reading/writing JSON into C++ data structures.
//  2. Excellent error messages while reading/writing data formats.
//  3. Versioning support. Fields/objects can be added without bumping the
//     minimum supported version.
//
// To achieve these goals there are some constraints:
//
//  * Every type used must be called out by the Archive API. Currently the only
//    stl container supported is std::vector.
//  * All classes in the scheme must implement a serialize() template method.
//  * Floating point numbers get their own category:
//     * Only double is supported for now (not float).
//     * Floating numbers can be NaN, +inf, -inf, but this is not strictly
//       valid JSON. If you want valid json, don't use these values.
//     * Floating point numbers print nicely to strings (e.g 1.1 instead of
//       1.999999999...) using the fantastic double-conersion library.
//
// Example client code:
//
//    #include <string>
//    #include <vector>
//    #include <easy_serialize/easy_serialize.h>
//
//    enum class EnumThing {
//        Thing_1,
//        Thing_2,
//        End,
//    };
//
//    const char* toString(EnumThing et)
//    {
//        switch (et) {
//        case Thing_0: return "thing 0";
//        case Thing_1: return "thing 1";
//        case End:     break;
//        }
//        return "";
//    }
//
//    class B
//    {
//    public:
//        B() : i(0) {}
//        B(int i) : i(i) {}
//        int i;
//
//        template<class Archive>
//        void serialize(Archive& ar) {
//            ar.doInt(i, "i");
//        }
//    };
//
//    class A
//    {
//    public:
//
//        bool b;
//        double d;
//        EnumThing et;
//        B o;
//        std::vector<B> v_o;
//        std::string s;
//        std::vector<int> v_i;
//
//        template<class Archive>
//        void serialize(Archive& ar)
//        {
//            ar.doBool(b, "b");
//            ar.doDouble(d, "d");
//            ar.doEnum(et, EnumThing::End, "et");
//            ar.doObject(o, "o");
//            ar.doString(s, "s");
//            ar.doVecInt(v_i, "v_i");
//            ar.doVecObject(v_o, "v_o");
//        }
//    };
//
//    int main(int, char*[])
//    {
//        A a;
//        a.b = true;
//        a.d = 3.14159265358979;
//        a.et = EnumThing::Thing_1;
//        a.o.i = 120;
//        a.v_o.emplace_back(1);
//        a.v_o.emplace_back(2);
//        a.s = "hello world";
//        a.v_i = {7,8,9};
//        try {
//            easy_serialize::to_json_stream(a, std::cout);
//        }
//        catch (const std::exception& ex) {
//            std::cerr << ex.what();
//        }
//        return 0;
//    }
//
//    ... prints this JSON ...
//
//    {
//      "b": true,
//      "d": 3.14159265358979,
//      "et": "thing 1",
//      "o": {
//        "i": 120
//      },
//      "s": "hello world",
//      "v_i": [
//        7,
//        8,
//        9
//      ],
//      "v_o": [
//        {
//          "i": 1
//        },
//        {
//          "i": 2
//        }
//      ]
//    }
//
//    ... likewise a call to easy_serialize::from_json_stream(...) will
//    populate data structures from JSON ...
//
//    std::ifstream ifs(filename);
//    if (!ifs.open()) {
//        std::cerr << "Couldn't open " << filename << std::endl;
//        return;
//    }
//    A a;
//    try {
//        from_json_stream(a, ifs);
//    }
//    catch (const std::exception& ex) {
//        std::cerr << ex.what();
//    }

namespace easy_serialize
{

    class JsonReaderArchive
    {
    public:
        JsonReaderArchive(std::istream& is);
        JsonReaderArchive(const JsonReaderArchive&) = delete;
        JsonReaderArchive& operator=(const JsonReaderArchive&) = delete;

        void doBool(bool&, const char* key);
        void doDouble(double&, const char* key);
        template<typename T>
        void doEnum(T&, T maxEnum, const char* key);
        void doInt(int& i, const char* key);
        template<typename T>
        void doObject(T& t, const char* key);
        void doString(std::string&, const char* key);
        void doVecFloat(std::vector<int>&, const char* key);
        void doVecInt(std::vector<int>&, const char* key);
        template<typename T>
        void doVecObject(std::vector<T>&, const char* key);

        // TODO:
        //void doVecDouble(std::vector<int>&, const char* key);
        //void doVecEnum(std::vector<T>&, const char* key);
        //void doVecString(std::vector<std::string>&, const char* key);

    private:

        //void checkKey(const char* key);

        JsonValue _root;
        std::vector<const JsonValue*> _stack;
    };

    std::string buildErrorKey(const char* key);
    std::string buildErrorIndex(size_t index);
    std::string buildErrorMessage(const char* key, const std::string& message);

    template<typename T>
    void getFromEnum(T& t, T maxEnum, const std::string& s, const char* key)
    {
        for (int i = 0; i < maxEnum; ++i) {
            T e = static_cast<T>(i);
            if (s == toString(e)) {
                t = e;
                return;
            }
        }
        const std::string error = " bad enum value: \"" + s + "\"";
        throw std::runtime_error(buildErrorKey(key) + error);
    }

    template<typename T>
    void JsonReaderArchive::doEnum(T& e, T maxEnum, const char* key)
    {
        std::string s;
        doString(s, key);
        getFromEnum(e, maxEnum, s, key);
    }

    template<typename T>
    void JsonReaderArchive::doObject(T& t, const char* key)
    {
        try {
            auto& thisJsonVal = *_stack.back();
            auto it = thisJsonVal.object.find(key);
            if (it == thisJsonVal.object.end()) {
                throw std::runtime_error(" key doesn't exist");
            }
            auto& nextJsonValue = it->second;
            if (nextJsonValue.jsonType != JsonType::Object) {
                throw std::runtime_error(" expected an object");
            }
            _stack.push_back(&nextJsonValue);
            t.serialize(*this);
            _stack.pop_back();
        }
        catch (const std::exception& ex) {
            const std::string error = buildErrorKey(key) + ex.what();
            throw std::runtime_error(error);
        }
    }

    template<class T>
    void JsonReaderArchive::doVecObject(std::vector<T>& v_o, const char* key)
    {
        try {
            auto& thisJsonVal = *_stack.back();
            auto it = thisJsonVal.object.find(key);
            if (it == thisJsonVal.object.end()) {
                throw std::runtime_error(" key doesn't exist");
            }
            auto& nextJsonValue = it->second;
            if (nextJsonValue.jsonType != JsonType::Array)
            {
                throw std::runtime_error(" expected an array");
            }
            
            v_o.clear();
            v_o.reserve(nextJsonValue.array.size());

            for (size_t i = 0; i < nextJsonValue.array.size(); ++i) {
                try {
                    if (nextJsonValue.array[i].jsonType != JsonType::Object) {
                        throw std::runtime_error(" expected an array of objects");
                    }
                    T o;
                    _stack.push_back(&nextJsonValue.array[i]);
                    o.serialize(*this);
                    _stack.pop_back();
                    v_o.push_back(o);
                }
                catch (const std::exception& ex) {
                    const std::string error = buildErrorIndex(i) + ex.what();
                    throw std::runtime_error(error);
                }
            }
            
        }
        catch (const std::exception& ex) {
            const std::string error = buildErrorKey(key) + ex.what();
            throw std::runtime_error(error);
        }
    }

    class JsonWriterArchive
    {
    public:
        JsonWriterArchive(std::ostream&, size_t indentSize=2);
        JsonWriterArchive(const JsonWriterArchive&) = delete;
        JsonWriterArchive& operator=(const JsonWriterArchive&) = delete;

        void doBool(bool&, const char* key);
        void doDouble(double&, const char* key);
        template<typename T>
        void doEnum(T& t, T maxEnum, const char* key);
        void doInt(int& i, const char* key);
        template<typename T>
        void doObject(T& t, const char* key);
        void doString(std::string&, const char* key);
        void doVecFloat(std::vector<float>&, const char* key);
        void doVecInt(std::vector<int>&, const char* key);
        template<typename T>
        void doVecObject(std::vector<T>&, const char* key);

    private:

        std::ostream& _osw;
        size_t _indentSize;
        std::string _colon_str;
        std::string _comma_str;
        std::vector<bool> _isFirstFieldStack;

        void writeKey(const char*);
    };

    void JsonWriterArchive::writeKey(const char* key)
    {
        if (!_isFirstFieldStack.back()) {
            _osw << ',';
            _isFirstFieldStack.back() = false;
        }
        if (_indentSize > 0) {
            _osw << std::endl;
            _osw << std::string(_indentSize * _isFirstFieldStack.size(), ' ');
        }
        _osw << '"' << key << '"' << _colon_str;
    }

    template<typename T>
    void JsonWriterArchive::doEnum(T& t, T /*maxEnum*/, const char* key)
    {
        writeKey(key);
        _osw << '"' << toString(t) << '"';
    }

    template<typename T>
    void JsonWriterArchive::doObject(T& o, const char* key)
    {
        writeKey(key);
        _osw << '{';
        _isFirstFieldStack.push_back(true);
    
        o.serialize(*this);
    
        if (!_isFirstFieldStack.back()) {
            _osw << std::endl;
            _osw << std::string(_indentSize * (_isFirstFieldStack.size()-1), ' ');
        }
        _isFirstFieldStack.pop_back();
        _osw << '}';
    }

    template<typename T>
    void JsonWriterArchive::doVecObject(std::vector<T>& v_o, const char* key)
    {
        writeKey(key);
        _isFirstFieldStack.push_back(true);
        //TODO get the spacing right in this B.
        _osw << '[';
        for (auto& o : v_o) {
            _osw << '{';
            _isFirstFieldStack.push_back(true);
            o.serialize(*this);
            _isFirstFieldStack.pop_back();
            _osw << '}';
        }
        _osw << ']';
        _isFirstFieldStack.pop_back();
    }

    template<typename T>
    void to_json_stream(T obj, std::ostream& os, size_t indentSize)
    {
        JsonWriterArchive a(os, indentSize);
        obj.serialize(a);
    }

    template<typename T>
    void from_json_stream(T& obj, std::istream& is)
    {
        JsonReaderArchive a(is);
        obj.serialize(a);
    }

} // namespace easy_serialize
