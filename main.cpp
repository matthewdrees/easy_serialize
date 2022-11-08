#include "easy_serialize/json_file_writer.hpp"
#include "easy_serialize/json_writer.hpp"

#include <iostream>

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
    uint32_t u32;
    uint64_t u64;
    bool b;
    double d;
    std::string s;
    OrangeJuicePulpLevel pulp_level;
    Y y;
    std::vector<Y> v_y;

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
    }
};

int main(int, char *[])
{
    Z json_object{127, -32768, 42, -9, 255, 65535, 196, 327, true, 0.1, "grr", OrangeJuicePulpLevel::Medium, {1, 2}, {{3, 4}, {5, 6}}};
    std::cout << easy_serialize::to_json_string(json_object, easy_serialize::JsonIndent::two_spaces) << "\n";
    {
        const auto status = easy_serialize::to_json_file("json_object.out", json_object);
        if (!status)
        {
            std::cerr << status.error_message << "\n";
        }
    }

    {
        std::vector<Z> vector_objects;
        vector_objects.push_back(json_object);
        vector_objects.push_back(json_object);
        std::cout << easy_serialize::to_json_string_vector_objects(vector_objects, easy_serialize::JsonIndent::two_spaces) << "\n";
        const auto status = easy_serialize::to_json_file_vector_objects("vector_objects.out", vector_objects);
        if (!status)
        {
            std::cerr << status.error_message << "\n";
        }
    }

    {
        std::vector<OrangeJuicePulpLevel> pulp_levels = {OrangeJuicePulpLevel::High, OrangeJuicePulpLevel::Medium, OrangeJuicePulpLevel::Low};
        std::cout << easy_serialize::to_json_string_vector_enums(pulp_levels, easy_serialize::JsonIndent::three_spaces) << "\n";
        const auto status = easy_serialize::to_json_file_vector_enums("pulp_levels.out", pulp_levels);
        if (!status)
        {
            std::cerr << status.error_message << "\n";
        }
    }

    {
        std::vector<int> ints = {3, 1, 4, 2, 8};
        std::cout << easy_serialize::to_json_string_vector(ints, easy_serialize::JsonIndent::four_spaces) << "\n";
        const auto status = easy_serialize::to_json_file_vector("ints.out", ints);
        if (!status)
        {
            std::cerr << status.error_message << "\n";
        }
        const auto status2 = easy_serialize::to_json_file_vector("imma/crazy/path/that/does/not/exist/json.out", ints);
        if (status2)
        {
            std::cout << "Expected error message: " << status2.error_message << "\n";
        }
    }

    return 0;
}
