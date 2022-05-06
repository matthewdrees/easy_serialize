
#include "easy_serialize/easy_serialize.h"
#include <rapidjson/error/en.h>

using namespace std;
using namespace rapidjson;

namespace easy_serialize
{
    string buildErrorKey(const char *key)
    {
        string s = "[\"";
        s += key;
        s += "\"]";
        return s;
    }

    string buildErrorMessage(const char *key, const string &message)
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

    void JsonReaderArchive::checkKey(const char *key)
    {
        if (!_stack.back().HasMember(key))
        {
            throw runtime_error(buildErrorMessage(key, " key doesn't exist"));
        }
    }

    JsonReaderArchive::JsonReaderArchive(std::string s) : json_string(s), _stack()
    {
        _d.Parse<kParseValidateEncodingFlag | kParseNanAndInfFlag>(&json_string[0], s.size());
        if (_d.HasParseError())
        {
            std::cout << "has parse error: " << rapidjson::GetParseError_En(_d.GetParseError()) << "\n";
        }
        if (!_d.IsObject())
        {
            throw runtime_error("root must be an object");
        }
        _stack.push_back(_d.GetObject());
    }

    JsonReaderArchive::~JsonReaderArchive()
    {
        _stack.pop_back();
    }

    void JsonReaderArchive::ez(const char *key, bool &b)
    {
        checkKey(key);
        if (!_stack.back()[key].IsBool())
        {
            throw runtime_error(buildErrorMessage(key, " expected a bool"));
        }
        b = _stack.back()[key].GetBool();
    }

    void JsonReaderArchive::doDouble(double &d, const char *key)
    {
        checkKey(key);
        if (!_stack.back()[key].IsDouble())
        {
            throw runtime_error(buildErrorMessage(key, " expected a double"));
        }
        d = _stack.back()[key].GetDouble();
    }

    void JsonReaderArchive::ez(const char *key, int &i)
    {
        checkKey(key);
        if (!_stack.back()[key].IsInt())
        {
            throw runtime_error(buildErrorMessage(key, " expected an int32"));
        }
        i = _stack.back()[key].GetInt();
    }

    void JsonReaderArchive::doString(std::string &s, const char *key)
    {
        checkKey(key);
        if (!_stack.back()[key].IsString())
        {
            throw runtime_error(buildErrorMessage(key, " expected a string"));
        }
        s = _stack.back()[key].GetString();
    }

    void JsonReaderArchive::doVecInt(std::vector<int> &v, const char *key)
    {
        checkKey(key);
        const Value &value = _stack.back()[key];
        if (!value.IsArray())
        {
            throw runtime_error(buildErrorMessage(key, " expected an array of integers"));
        }

        v.clear();
        v.reserve(value.Size());

        for (SizeType i = 0; i < value.Size(); ++i)
        {
            const Value &a = value[i];
            if (!a.IsInt())
            {
                const string error = "[" + to_string(i) + "] expected an integer";
                throw runtime_error(buildErrorMessage(key, error));
            }
            v.push_back(a.GetInt());
        }
    }

    JsonWriterArchive::JsonWriterArchive(ostream &os, bool is_object) : _is_object(is_object),
                                                                        _osw(os),
                                                                        _writer(_osw)
    {
        _writer.SetIndent(' ', 2);
        if (_is_object)
        {
            _writer.StartObject();
        }
        else
        {
            _writer.StartArray();
        }
    }

    JsonWriterArchive::~JsonWriterArchive()
    {
        if (_is_object)
        {
            _writer.EndObject();
        }
        else
        {
            _writer.EndArray();
        }
    }

    void JsonWriterArchive::ez(const char *key, bool &b)
    {
        _writer.Key(key);
        _writer.Bool(b);
    }

    void JsonWriterArchive::doDouble(double &d, const char *key)
    {
        _writer.Key(key);
        _writer.Double(d);
    }

    void JsonWriterArchive::ez(const char *key, int &i)
    {
        _writer.Key(key);
        _writer.Int(i);
    }

    void JsonWriterArchive::doString(string &s, const char *key)
    {
        _writer.Key(key);
        _writer.String(s);
    }

    void JsonWriterArchive::doVecInt(vector<int> &v, const char *key)
    {
        _writer.Key(key);
        _writer.StartArray();
        for (const auto i : v)
        {
            _writer.Int(i);
        }
        _writer.EndArray();
    }

} // namespace easy_serialize
