// Copyright (c) 2016 Matt Drees, Fluke Networks

#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "easy_serialize/easy_serialize.h"

using namespace std;
using namespace easy_serialize;

enum EnumThing {
    ENUM_THING_0,
    ENUM_THING_1,
    ENUM_THING_N,
};

const char* toString(EnumThing et)
{
    switch (et) {
    case ENUM_THING_0: return "enum thing 0";
    case ENUM_THING_1: return "enum thing 1";
    case ENUM_THING_N: break;
    }
    return "";
}

class B
{
public:
    int x;
    string s;
    std::vector<int> v_i;

    template<class Archive>
    void serialize(Archive& ar);
};

template<class Archive>
void B::serialize(Archive& ar)
{
    ar.doInt(x, "x");
    ar.doString(s, "s");
    ar.doVecInt(v_i, "v_i");
}

class A
{
public:

    template<class Archive>
    void serialize(Archive& ar);

    bool b;
    double d;
    int i;
    EnumThing et;
    B o;
    vector<B> v_o;
};

template<class Archive>
void A::serialize(Archive& ar)
{
    //const int CLASS_VERSION = 0;
    //ar.doHeader("A", CLASS_VERSION);
    ar.doBool(b, "b");
    ar.doDouble(d, "d");
    ar.doInt(i, "i");
    ar.doObject(o, "o");
    ar.doEnum(et, ENUM_THING_N, "et");
    ar.doVecObject(v_o, "v_o");
}

void validate_exception(const char* file,
                        int line,
                        const std::string& json,
                        const std::string& exception,
                        int& num_tests_passed)
{
    try {
        istringstream iss(json);
        A a;
        from_json_stream(a, iss);
        to_json_stream(a, cout);
    }
    catch (const std::exception& ex)
    {
        if (ex.what() != exception) {
            cerr << endl << file << ":" << line
                 << ", expected exception \"" << exception
                 << "\", found \"" << ex.what() << "\"" << endl;
        }
        else {
            cout << ".";
            ++num_tests_passed;
        }

        return;
    }

    cerr << endl << file << ":" << line
         << ", expected exception \"" << exception
         << "\" but none thrown" << endl;
}

#define VALIDATE_EXCEPTION(json, exception_str) \
    ++num_tests; \
    validate_exception(__FILE__, __LINE__, json, exception_str, num_tests_passed)

void validate_json(const char* file,
                   int line,
                   const std::string& json,
                   int& num_tests_passed)
{
    try {
        istringstream iss(json);
        A a;
        from_json_stream(a, iss);
        ostringstream oss;
        to_json_stream(a, oss);

        auto actual = oss.str();
        if (actual == json) {
            ++num_tests_passed;
            return;
        }
        else {
            cerr << endl << file << ":" << line
                 << ", expected json:" << endl
                 << json << endl
                 << "found: json:" << endl
                 << actual << "\"" << endl;
        }
    }
    catch (const std::exception& ex) {
        cerr << endl << file << ":" << line
             << ", didn't expect exception, found \""
             << ex.what() << "\"" << endl;
    }
}

#define VALIDATE_JSON(json) \
    ++num_tests; \
    validate_json(__FILE__, __LINE__, json, num_tests_passed)

int main()
{
    cout << "Testing " << __FILE__ << ": ";

    int num_tests = 0;
    int num_tests_passed = 0;

    {
        const string json = "{\n"
            "  \"b\": true,\n"
            "  \"d\": 3.14159265358979,\n"
            "  \"i\": 9,\n"
            "  \"o\": {\n"
            "    \"x\": 8,\n"
            "    \"s\": \"a string\",\n"
            "    \"v_i\": [\n"
            "      1,\n"
            "      2,\n"
            "      3\n"
            "    ]\n"
            "  },\n"
            "  \"et\": \"enum thing 1\",\n"
            "  \"v_o\": [\n"
            "    {\n"
            "      \"x\": 65535,\n"
            "      \"s\": \"a string\",\n"
            "      \"v_i\": []\n"
            "    },\n"
            "    {\n"
            "      \"x\": 65534,\n"
            "      \"s\": \"b string\",\n"
            "      \"v_i\": [\n"
            "        3\n"
            "      ]\n"
            "    }\n"
            "  ]\n"
            "}";
        VALIDATE_JSON(json);
    }

    {
        const string json = "[]";
        VALIDATE_EXCEPTION(json, "root must be an object");
    }

    {
        const string json = "{\n"
            "  \"b\": 0,\n"
            "  \"d\": 2.2,\n"
            "  \"i\": 9,\n"
            "  \"o\": {\n"
            "    \"x\": 8,\n"
            "    \"s\": \"s\",\n"
            "    \"v_i\": [1, 2, 3]\n"
            "  },\n"
            "  \"et\": \"enum thing 0\"\n"
            "}";
        VALIDATE_EXCEPTION(json, "[\"b\"] expected a bool");
    }

    {
        const string json = "{\n"
            "  \"b\": {\"grr\": true},\n"
            "  \"d\": 4.4,\n"
            "  \"i\": 9,\n"
            "  \"o\": {\n"
            "    \"x\": 8,\n"
            "    \"s\": \"s\",\n"
            "    \"v_i\": [1, 2, 3]\n"
            "  },\n"
            "  \"et\": \"enum thing 0\"\n"
            "}";
        VALIDATE_EXCEPTION(json, "[\"b\"] expected a bool");
    }

    {
        const string json = "{\n"
            "  \"b\": false,\n"
            "  \"d\": 6.6,\n"
            "  \"i\": true,\n"
            "  \"o\": {\n"
            "    \"x\": 8,\n"
            "    \"s\": \"s\",\n"
            "    \"v_i\": [1, 2, 3]\n"
            "  },\n"
            "  \"et\": \"enum thing 0\"\n"
            "}";
        VALIDATE_EXCEPTION(json, "[\"i\"] expected an int");
    }

    {
        const string json = "{\n"
            "  \"b\": false,\n"
            "  \"d\": 8.8,\n"
            "  \"i\": 1000,\n"
            "  \"o\": 7,\n"
            "  \"et\": \"enum thing 0\"\n"
            "}";
        VALIDATE_EXCEPTION(json, "[\"o\"] expected an object");
    }

    {
        const string json = "{\n"
            "  \"i\": true,\n"
            "  \"d\": 8.8,\n"
            "  \"o\": {\n"
            "    \"x\": 8,\n"
            "    \"s\": \"s\",\n"
            "    \"v_i\": [1, 2, 3]\n"
            "  },\n"
            "  \"et\": \"enum thing 0\"\n"
            "}";
        VALIDATE_EXCEPTION(json, "[\"b\"] key doesn't exist");
    }

    {
        const string json = "{\n"
            "  \"b\": false,\n"
            "  \"d\": 8.8,\n"
            "  \"i\": 7,\n"
            "  \"et\": \"enum thing 0\"\n"
            "}";
        VALIDATE_EXCEPTION(json, "[\"o\"] key doesn't exist");
    }

    {
        const string json = "{\n"
            "  \"b\": false,\n"
            "  \"d\": 1.234567890123,\n"
            "  \"i\": 7,\n"
            "  \"o\": {\n"
            "    \"x\": 8,\n"
            "    \"s\": \"s\",\n"
            "    \"v_i\": [1, 2, 3]\n"
            "  },\n"
            "  \"et\": {\"grr\": true}\n"
            "}";
        VALIDATE_EXCEPTION(json, "[\"et\"] expected a string");
    }

    {
        const string json = "{\n"
            "  \"b\": false,\n"
            "  \"d\": 1.234567890123,\n"
            "  \"i\": 7,\n"
            "  \"o\": {\n"
            "    \"x\": 8,\n"
            "    \"s\": \"s\",\n"
            "    \"v_i\": [1, 2, 3]\n"
            "  },\n"
            "  \"et\": \"not an enum thing\"\n"
            "}";
        VALIDATE_EXCEPTION(json, "[\"et\"] bad enum value: \"not an enum thing\"");
    }

    {
        const string json = "{\n"
            "  \"b\": false,\n"
            "  \"d\": 1.234567890123,\n"
            "  \"i\": 7,\n"
            "  \"o\": {\n"
            "    \"x\": 8,\n"
            "    \"s\": \"s\",\n"
            "    \"v_i\": 0.1\n"
            "  },\n"
            "  \"et\": \"not an enum thing\"\n"
            "}";
        VALIDATE_EXCEPTION(json, "[\"o\"][\"v_i\"] expected an array of integers");
    }

    {
        const string json = "{\n"
            "  \"b\": false,\n"
            "  \"d\": 1.234567890123,\n"
            "  \"i\": 7,\n"
            "  \"o\": {\n"
            "    \"x\": 8,\n"
            "    \"s\": \"s\",\n"
            "    \"v_i\": [7, 0.1]\n"
            "  },\n"
            "  \"et\": \"not an enum thing\"\n"
            "}";
        VALIDATE_EXCEPTION(json, "[\"o\"][\"v_i\"][1] expected an integer");
    }

    {
        const string json = "{\n"
            "  \"b\": true,\n"
            "  \"d\": 1.234567890123,\n"
            "  \"i\": 9,\n"
            "  \"o\": {\n"
            "    \"x\": 8,\n"
            "    \"s\": \"a string\",\n"
            "    \"v_i\": []\n"
            "  },\n"
            "  \"et\": \"enum thing 1\"\n"
            "}";
        VALIDATE_EXCEPTION(json, "[\"v_o\"] key doesn't exist");
    }

    {
        const string json = "{\n"
            "  \"b\": true,\n"
            "  \"d\": 1.234567890123,\n"
            "  \"i\": 9,\n"
            "  \"o\": {\n"
            "    \"x\": 8,\n"
            "    \"s\": \"a string\",\n"
            "    \"v_i\": []\n"
            "  },\n"
            "  \"et\": \"enum thing 1\",\n"
            "  \"v_o\": [\n"
            "    {\n"
            "      \"x\": 65535,\n"
            "      \"s\": \"a string\",\n"
            "      \"v_i\": []\n"
            "    },\n"
            "    true\n"
            "  ]\n"
            "}";
        VALIDATE_EXCEPTION(json, "[\"v_o\"][1] expected an object");
    }

    {
        const string json = "{\n"
            "  \"b\": true,\n"
            "  \"d\": 1.234567890123,\n"
            "  \"i\": 9,\n"
            "  \"o\": {\n"
            "    \"x\": 8,\n"
            "    \"s\": \"a string\",\n"
            "    \"v_i\": []\n"
            "  },\n"
            "  \"et\": \"enum thing 1\",\n"
            "  \"v_o\": [\n"
            "    {\n"
            "      \"x\": 65535,\n"
            "      \"s\": 3,\n"
            "      \"v_i\": []\n"
            "    }\n"
            "  ]\n"
            "}";
        VALIDATE_EXCEPTION(json, "[\"v_o\"][0][\"s\"] expected a string");
    }

    cout << " " << num_tests_passed << "/" << num_tests
         << " tests passed!" << endl;
    return num_tests != num_tests_passed;
}

