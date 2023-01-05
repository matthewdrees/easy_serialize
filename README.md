# easy_serialize

easy_serialize is a header-only >= C++14 library for those that want their C++ objects to/from JSON without much fuss. (And possibly other data formats in the future.)

# Easy to read and write JSON

```
    #include "easy_serialize/json_reader.hpp"
    #include "easy_serialize/json_writer.h"

    enum class OrangeJuicePulpLevel {
        Low,
        Medium,
        High,
        N // One past last valid value. (Have to have this.)
    };

    // Client has to define a to_string() function for enum values.
    const char* to_string(OrangeJuicePulpLevel level)
    {
        switch (level) {
        case Low: return "low";
        case Medium: return "medium";
        case High: return "high";
        case N: break;
        }
        return "";
    }

    class Y
    {
      public:
        int i;
        int64_t i2;

        // Client adds this method. Handles reading and writing.
        template<class Archive>
        void serialize(Archive& ar)
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

        // Read the above JSON string and populate an object.
        Z z;
        const auto status = easy_serialize::from_json_string(json, z);
        if (!status)
        {
            std::cerr << status.get_error_message() << "\n";
            return 1;
        }

        // Write the object json to stdout. (Same as string above.)
        std::cout << easy_serialize::to_json_string(z) << "\n";
        return 0;
    }
```

# Easy to find errors in your JSON

Errors while reading JSON give you a location and error message. Examples:

 * ["v_y"][1]["i2"] expected an int64
 * ["d"] key doesn't exist
 * ["pulp level"] expected an enum value

TODO: The default implementation uses rapidjson for UTF-8 validation and parse errors. If one of these errors happens it gives you error messages like "invalid encoding in string" or "Missing a closing quotation mark in string.", but doesn't give you an exact location, which would be nice.

# Object versioning

Example with object versioning.

```
    class W
    {
    public:

        // In original version (version 0 implied)
        int i0;

        // Added in version 1
        int i1;
        std::string s;

        // Added in version 2
        int i2;

        template<class Archive>
        void serialize(Archive& ar)
        {
            ar.class_version(2) // Enables versioning, with current class version = 2.
            ar.ez("i0", i0); // Supported object version is assumed zero with the usual call.

            // Version 1
            ar.ez("i1", i1, 1); // Note: Using methods with supported_object_version parameter.
            ar.ez("s", s, 1);

            // Version 2 
            ar.ez("i2", i2, 2);
        }
    };
```

For json archivers this adds a "_objver": int key/value pair to the json if the class version is > 0.

```

    // JSON for class above for different object versions.
    {
      "i1": 0
    }

    {
      "_objver": 1,
      "i1": 0,
      "i2": 0,
      "s": ""
    }

    {
      "_objver": 2,
      "i1": 0,
      "i2": 0,
      "s": "",
      "i3": 0
    }
```

You can only add members with a version change (not remove existing members). A possible modification in the future would be to add a function that can remember the deserialized object version so the client could do further processing.

For binary archivers (that don't exist yet) it will include an integer version field.

# Adding another archiver

Currently there is only a JSON archiver based on rapidjson.

Possible future archivers:
* Binary (using variable-length numbers similar to protocol buffers).
* Customized json archiver tailored to this scheme (rather than rapidjson). It could have better performance and error reporting.
* Equality operator (For floating point numbers, NaN == NaN is true).

Use the rapidjson implementation as a guide.

# Type support

The following types are supported:
* bool
* int8_t
* int16_t
* int32_t
* int64_t
* uint8_t
* uint16_t
* uint32_t
* uint64_t
* double (supports NaN, Inf, Infinity, -Inf, and -Infinity)
* std::string
* enum and enum classes with a to_string function
* classes/structs with a serialize method
* std::vector

Not supported:
* float (but yes to support for double)
* pointers
* polymorphism
* classes/structs without a serialize method (must be intrusive)
* vectors of vectors
# Test coverage

Built and tested with -Wpedantic, -Wshadow -Wextra -Wconversion -Wunused -Wshadow -Werror -fsanitize=address, undefined.

v1.0.0:
* g++ 9.3.0 --std=c++14, c++17, c++2a on Ubuntu 20.04.
* clang 11.0.0 --std=c++14, c++17, c++20 on MacOS 12.6
* Visual Studio 2022 (Compiler Version 19.32.31332)
