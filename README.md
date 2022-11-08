# easy_serialize

easy_serialize is a header-only C++11 library for those that want their C++ objects to/from JSON without much fuss. (And possibly other data formats in the future.)

# Easy to write some JSON

    #include <easy_serialize/json_writer.h>

    enum class OrangeJuicePulpLevel {
        Low,
        Medium,
        High,
        N // One past last valid value. (Have to have this.)
    };

    // Client has to provide to_string() function for enum values.
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

        bool b;
        double d;
        std::string s;
        OrangeJuicePulpLevel pulp_level;
        Y y;
        std::vector<Y> v_y;

        // Client adds this method. Handles reading and writing.
        template<class Archive>
        void serialize(Archive& ar)
        {
            ar.ez("b", b);
            ar.ez("d", d);
            ar.ez("s", s);
            ar.ez_enum("pulp level", pulp_level, OrangeJuicePulpLevel::N);
            ar.ez_obj("y", y);
            ar.ez_vec("v_y", v_y);
        }
    };

    int main(int, char*[])
    {
        Z z { true, 0.1, "grr", OrangeJuicePulpLevel::medium, {1, 2}, {{3,4}, {5,6}}};
        std::cout << to_json_string(z);
    }

    ... prints this JSON (the default indent level is 2 spaces) ...

    {
      "b": true,
      "d": 0.1,
      "pulp level": "medium",
      "s": "grr",
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
      ]
    }

# Easy to read some JSON

    With the same code from above you can read json into your objects.

    #include <easy_serialize/json_reader.h>
    #include <iostream>

    int main(int, char*[])
    {
        const std::string json_string = R""""(
        {
          "b": false,
          "d": 0.2,
          "pulp level": "low",
          "s": "more grr",
          "y": {
            "i": 7,
            "i2": 8
          },
          "v_y": [
            {
              "i": 9,
              "i2": 10 
            },
            {
              "i": 11,
              "i2": 12 
            }
          ]
        }
        """");

        Z z;
        const auto status = easy_serialize::from_json_string(json_string, z);
        if (status) {
            // Object "z" is populated with json. Do something with it...
        }
        else {
            std::cerr << status.error_message << "\n";
        }

        return 0;
    }

# Easy to find errors in your JSON

Let's say one of the nested JSON values above was the wrong type. The program above would produce this error message...

    ["v_y"][1]["i2"] expected an int64

... or the "d" key didn't exist ...

    ["d"] key doesn't exist

... or an enum value didn't exist ...

    ["pulp level"] bad enum value: "just the pulp"

... and many others.

TODO: The default implementation uses rapidjson for UTF-8 validation and parse errors. If one of these errors happens it gives you error messages like "invalid encoding in string" or "Missing a closing quotation mark in string.", but doesn't give you an exact location, which would be nice.

# Class versioning

Use the archive class_version(int) and object_version_supported(int) functions for versioning. 

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
            ar.class_version(2) // Enables class versioning.
            ar.ez("i0", i0);

            ar.object_version_supported(1)
            ar.ez("i1", i1);
            ar.ez("s", s);

            ar.object_version_supported(2)
            ar.ez("i2", i2);
        }
    };

For json archivers this adds a "_objver": int key/value pair to the json if the class version is > 0.

    {
      "_objver": 2,
      "i1": 0,
      "i2": 0,
      "s": "",
      "i3": 0
    }

This way older objects such as these can still be read without a missing key error:

    {
      "_objver": 0,
      "i1": 0
    }

    {
      "_objver": 1,
      "i1": 0,
      "i2": 0,
      "s": ""
    }

You can only add members with a version change. A possible modification in the future would be to add a function that can remember the deserialized object version so the client could do further processing.

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
* enum and enum classes with a to_string function.
* classes/structs with a serialize method.
* std::vector

Not supported:
* pointers
* classes/structs without a serialize method.
