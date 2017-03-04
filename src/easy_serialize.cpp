// Copyright (c) 2016 Matt Drees, Fluke Networks

#include "easy_serialize/easy_serialize.h"

using namespace std;

namespace easy_serialize
{
    string buildErrorKey(const char* key)
    {
        string s = "[\"";
        s += key;
        s += "\"]";
        return s;
    }

    string buildErrorMessage(const char* key, const string&  message)
    {
        string s = buildErrorKey(key);
        s += message;
        return s;
    }

    string buildErrorIndex(unsigned key)
    {
        string s = "[" + to_string(key) + "]";
        return s;
    }

    JsonReaderArchive::JsonReaderArchive(istream& is) :
        _root(is),
        _stack(1, &_root)
    {
        //_stack.push_back(&_root);
    }

    void JsonReaderArchive::doBool(bool& b, const char* key)
    {
        auto& thisJsonVal = *_stack.back();
        auto it = thisJsonVal.object.find(key);
        if (it == thisJsonVal.object.end()) {
            throw runtime_error(buildErrorMessage(key, " key doesn't exist"));
        }
        auto& nextJsonValue = it->second;
        if (nextJsonValue.field == "true") {
            b = true;
        }
        else if (nextJsonValue.field == "false") {
            b = false;
        }
        else
        {
            throw std::runtime_error(" expected an object");
        }
    }

    void JsonReaderArchive::doDouble(double& d, const char* key)
    {
        auto& thisJsonVal = *_stack.back();
        auto it = thisJsonVal.object.find(key);
        if (it == thisJsonVal.object.end()) {
            throw runtime_error(buildErrorMessage(key, " key doesn't exist"));
        }
        auto& nextJsonValue = it->second;
        // TODO: actually get a number
        if (nextJsonValue.field == "true") {
            d = 1.0;
        }
        else
        {
            throw std::runtime_error(" expected a number");
        }
    }

    void JsonReaderArchive::doInt(int& i, const char* key)
    {
        auto& thisJsonVal = *_stack.back();
        auto it = thisJsonVal.object.find(key);
        if (it == thisJsonVal.object.end()) {
            throw runtime_error(buildErrorMessage(key, " key doesn't exist"));
        }
        auto& nextJsonValue = it->second;
        try {
            i = stoi(nextJsonValue.field);
        }
        catch (const std::invalid_argument&) {
            throw std::runtime_error(" expected an integer");
        }
        catch (const std::out_of_range&) {
            throw std::runtime_error(" integer out of range");
        }
    }

    void JsonReaderArchive::doString(std::string& s, const char* key)
    {
        auto& thisJsonVal = *_stack.back();
        auto it = thisJsonVal.object.find(key);
        if (it == thisJsonVal.object.end()) {
            throw runtime_error(buildErrorMessage(key, " key doesn't exist"));
        }
        auto& nextJsonValue = it->second;
        if (nextJsonValue.jsonType == JsonType::Field) {
            s = std::move(nextJsonValue.field);
        }
        else {
            throw runtime_error(buildErrorMessage(key, " expected a string"));
        }
    }

    void JsonReaderArchive::doVecInt(std::vector<int>& v, const char* key)
    {
        auto& thisJsonVal = *_stack.back();
        auto it = thisJsonVal.object.find(key);
        if (it == thisJsonVal.object.end()) {
            throw runtime_error(buildErrorMessage(key, " key doesn't exist"));
        }
        auto& nextJsonValue = it->second;
        if (nextJsonValue.jsonType == JsonType::Array) {
            v.clear();
            v.reserve(nextJsonValue.array.size());

            for (size_t i = 0; i < nextJsonValue.array.size(); ++i) {
                int number;
                try {
                    number = stoi(nextJsonValue.field);
                }
                catch (const std::invalid_argument&) {
                    const string error = "[" + to_string(i) + "] expected an integer";
                    throw runtime_error(buildErrorMessage(key, error));
                }
                catch (const std::out_of_range&) {
                    const string error = "[" + to_string(i) + "] integer out of range";
                    throw runtime_error(buildErrorMessage(key, error));
                }
                v.push_back(number);
            }
        }
        else {
            throw runtime_error(buildErrorMessage(key, " expected an array of integers"));
        }
    }

    JsonWriterArchive::JsonWriterArchive(ostream& os, size_t indentSize) :
        _osw(os),
        _indentSize(indentSize)
    {
        if (indentSize > 0) {
            _colon_str = ": ";
            _comma_str = ", ";
        }
        else {
            _colon_str = ":";
            _comma_str = ",";
        }
        _isFirstFieldStack.push_back(true);
    }

    JsonWriterArchive::~JsonWriterArchive()
    {
        _isFirstFieldStack.pop_back(); // Not strictly necessary
    }

    void JsonWriterArchive::doBool(bool& b, const char* key)
    {
        _writer.Key(key);
        _writer.Bool(b);
    }

    void JsonWriterArchive::doDouble(double& d, const char* key)
    {
        _writer.Key(key);
        _writer.Double(d);
    }

    void JsonWriterArchive::doInt(int& i, const char* key)
    {
        _writer.Key(key);
        _writer.Int(i);
    }

    void JsonWriterArchive::doString(string& s, const char* key)
    {
        _writer.Key(key);
        _writer.String(s);
    }

    void JsonWriterArchive::doVecInt(vector<int>& v, const char* key)
    {
        _writer.Key(key);
        _writer.StartArray();
        for (const auto i : v) {
            _writer.Int(i);
        }
        _writer.EndArray();
    }

} // namespace easy_serialize

