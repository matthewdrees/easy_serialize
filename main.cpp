// Sample easy_serialize usage.

#include "easy_serialize/json_reader.hpp"
#include "easy_serialize/json_writer.hpp"

#include <iostream>
#include <vector>

enum OrangeJuicePulpLevel
{
    Low,
    Medium,
    High,
    N // One past last valid value. (Have to have this.)
};

// Client has to provide to_string() function for enum values.
const char *to_string(OrangeJuicePulpLevel level)
{
    switch (level)
    {
    case OrangeJuicePulpLevel::Low:
        return "low";
    case OrangeJuicePulpLevel::Medium:
        return "medium";
    case OrangeJuicePulpLevel::High:
        return "high";
    case OrangeJuicePulpLevel::N:
        break;
    }
    return "";
}

class Y
{
public:
    int i;
    int64_t i2;

    // Client adds this method. Handles reading and writing.
    template <class Archive>
    void serialize(Archive &ar)
    {
        ar.ez("i", i);
        ar.ez("i2", i2);
    }
};

class Z
{
public:
    int8_t i8;
    int16_t i16;
    int32_t i32;
    int64_t i64;
    uint8_t u8;
    uint16_t u16;
    uint32_t u32 = 0;
    uint64_t u64;
    bool b;
    double d;
    std::string s;
    OrangeJuicePulpLevel pulp_level;
    Y y;
    std::vector<Y> v_y;
    std::vector<OrangeJuicePulpLevel> v_e;
    std::vector<std::string> v_s;

    // Client adds this method. Handles reading and writing.
    template <class Archive>
    void serialize(Archive &ar)
    {
        ar.ez("i8", i8);
        ar.ez("i16", i16);
        ar.ez("i32", i32);
        ar.ez("i64", i64);
        ar.ez("u8", u8);
        ar.ez("u16", u16);
        ar.ez("u32", u32);
        ar.ez("u64", u64);
        ar.ez("b", b);
        ar.ez("d", d);
        ar.ez("s", s);
        ar.ez_enum("pulp level", pulp_level, OrangeJuicePulpLevel::N);
        ar.ez_object("y", y);
        ar.ez_vector_objects("v_y", v_y);
        ar.ez_vector_enums("v_e", v_e, OrangeJuicePulpLevel::N);
        ar.ez_vector("v_s", v_s);
    }
};

int main(int, char *[])
{
    const std::string json = R"zzz({
  "i8": 127,
  "i16": -32768,
  "i32": 42,
  "i64": -9,
  "u8": 255,
  "u16": 65535,
  "u32": 196,
  "u64": 327,
  "b": true,
  "d": 0.1,
  "s": "grr",
  "pulp level": "medium",
  "y": {
    "i": 1,
    "i2": 2
  },
  "v_y": [
    {
      "i": 3,
      "i2": 4
    },
    {
      "i": 5,
      "i2": 6
    }
  ],
  "v_e": [
    "medium",
    "high",
    "low"
  ],
  "v_s": [
    "we",
    "are",
    "strings"
  ]
})zzz";
    Z z;
    const auto status = easy_serialize::from_json_string(json, z);
    if (!status)
    {
        std::cerr << status.get_error_message() << "\n";
        return 1;
    }
    const std::string json2 = easy_serialize::to_json_string(z);
    std::cout << json2 << "\n";

    if (json == json2)
    {
        std::cout << "Read/write JSON strings are equal.\n";
    }
    else
    {
        std::cout << "Read/write JSON strings are NOT equal.\n";
        std::cout << json << "\n";
    }
    return 0;
}
