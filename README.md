# easy_serialize

easy_serialize is a C++11 library and a client usage pattern with the following goals:

1. Easy to use for reading/writing JSON into C++ data structures.
2. No virtual inheritance.
3. Excellent error messages while reading/writing data formats.
4. Versioning support. Fields/objects can be added without bumping the minimum supported version.

To achieve these goals there are some constraints:

* Every type used must be called out by the Archive API. Currently the only stl container supported is std::vector.
* All classes in the scheme must implement a serialize() template method.
* Floating point numbers get their own category:
  * Only double is supported for now (not float).
  * Floating point numbers can't be NaN, +inf, or -inf.
  * Floating point numbers print nicely to strings (e.g 1.1 instead of 1.999999999...). This means that floating point numbers aren't preserved exactly in a to/from JSON operation, and get filtered by what can display in a decimal string.

Example client code:

    #include <string>
    #include <vector>
    #include <easy_serialize/easy_serialize.h>
 
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
        B() : i(0) {}
        B(int i) : i(i) {}
        int i;
 
        template<class Archive>
        void serialize(Archive& ar) {
            ar.doInt(i, "i");
        }
    };
 
    class A
    {
    public:
 
        bool b;
        double d;
        EnumThing et;
        B o;
        std::vector<B> v_o;
        std::string s;
        std::vector<int> v_i;
 
        template<class Archive>
        void serialize(Archive& ar)
        {
            ar.doBool(b, "b");
            ar.doDouble(d, "d");
            ar.doEnum(et, ENUM_THING_N, "et");
            ar.doObject(o, "o");
            ar.doString(s, "s");
            ar.doVecInt(v_i, "v_i");
            ar.doVecObject(v_o, "v_o");
        }
    };
 
    int main(int, char*[])
    {
        A a;
        a.b = true;
        a.d = 3.14159265358979;
        a.et = ENUM_THING_1;
        a.o.i = 120;
        a.v_o.emplace_back(1);
        a.v_o.emplace_back(2);
        a.s = "hello world";
        a.v_i = {7,8,9};
        try {
            easy_serialize::to_json_stream(a, std::cout);
        }
        catch (const std::exception& ex) {
            std::cerr << ex.what();
        }
        return 0;
    }
 
    ... prints this JSON ...
 
    {
      "b": true,
      "d": 3.14159265358979,
      "et": "enum thing 1",
      "o": {
        "i": 120
      },
      "s": "hello world",
      "v_i": [
        7,
        8,
        9
      ],
      "v_o": [
        {
          "i": 1
        },
        {
          "i": 2
        }
      ]
    }
 
    ... likewise a call to easy_serialize::from_json_stream(...) will
    populate data structures from JSON ...
 
    std::ifstream ifs(filename);
    if (!ifs.open) {
        std::cerr << "Couldn't open " << filename << std::endl;
        return;
    }
    A a;
    try {
        from_json_stream(a, ifs);
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what();
    }
 
 
