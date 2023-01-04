// Unit tests for easy_serialize library.

#include "easy_serialize/json_file_reader.hpp"
#include "easy_serialize/json_file_writer.hpp"
#include "easy_serialize/json_reader.hpp"
#include "easy_serialize/json_writer.hpp"

#include <iostream>
#include <limits>
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
  double d = 0.0;
  double d2 = 0.0;

  // Client adds this method. Handles reading and writing.
  template <class Archive>
  void serialize(Archive &ar)
  {
    ar.ez("d", d);
    ar.ez("d2", d2);
  }
};

class Z
{
public:
  int8_t i8 = 0;
  int16_t i16 = 0;
  int32_t i32 = 0;
  int64_t i64 = 0;
  uint8_t u8 = 0;
  uint16_t u16 = 0;
  uint32_t u32 = 0;
  uint64_t u64 = 0;
  bool b = false;
  double d = 0.0;
  std::string s;
  OrangeJuicePulpLevel pulp_level = OrangeJuicePulpLevel::Low;
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

int test_writer_mins()
{
  Z z;
  z.i8 = std::numeric_limits<int8_t>::min();
  z.i16 = std::numeric_limits<int16_t>::min();
  z.i32 = std::numeric_limits<int32_t>::min();
  z.i64 = std::numeric_limits<int64_t>::min();
  z.u8 = std::numeric_limits<uint8_t>::min();
  z.u16 = std::numeric_limits<uint16_t>::min();
  z.u32 = std::numeric_limits<uint32_t>::min();
  z.u64 = std::numeric_limits<uint64_t>::min();
  z.b = false;
  z.d = std::numeric_limits<double>::min();
  z.y.d = std::numeric_limits<double>::quiet_NaN();
  z.y.d2 = -std::numeric_limits<double>::infinity();
  std::string actual = easy_serialize::to_json_string(z);
  const std::string expected = R"zzz({
  "i8": -128,
  "i16": -32768,
  "i32": -2147483648,
  "i64": -9223372036854775808,
  "u8": 0,
  "u16": 0,
  "u32": 0,
  "u64": 0,
  "b": false,
  "d": 2.2250738585072014e-308,
  "s": "",
  "pulp level": "low",
  "y": {
    "d": NaN,
    "d2": -Infinity
  },
  "v_y": [],
  "v_e": [],
  "v_s": []
})zzz";
  if (expected != actual)
  {
    std::cerr << __FILE__ << ":" << __LINE__ << ", FAIL, expected: " << expected
              << "\nactual: " << actual << "\n";
    return 1;
  }
  return 0;
}

bool test_writer_maxes()
{
  Z z;
  z.i8 = std::numeric_limits<int8_t>::max();
  z.i16 = std::numeric_limits<int16_t>::max();
  z.i32 = std::numeric_limits<int32_t>::max();
  z.i64 = std::numeric_limits<int64_t>::max();
  z.u8 = std::numeric_limits<uint8_t>::max();
  z.u16 = std::numeric_limits<uint16_t>::max();
  z.u32 = std::numeric_limits<uint32_t>::max();
  z.u64 = std::numeric_limits<uint64_t>::max();
  z.b = true;
  z.d = std::numeric_limits<double>::max();
  z.s = "this is a string";
  z.pulp_level = OrangeJuicePulpLevel::High;
  z.y.d = std::numeric_limits<double>::lowest();
  z.y.d2 = std::numeric_limits<double>::infinity();
  z.v_y = {{0.0, -std::numeric_limits<double>::lowest()}};
  z.v_e = {OrangeJuicePulpLevel::High, OrangeJuicePulpLevel::Medium, OrangeJuicePulpLevel::Low};
  z.v_s = {"strings", "", "1"};
  std::string actual = easy_serialize::to_json_string(z);
  const std::string expected = R"zzz({
  "i8": 127,
  "i16": 32767,
  "i32": 2147483647,
  "i64": 9223372036854775807,
  "u8": 255,
  "u16": 65535,
  "u32": 4294967295,
  "u64": 18446744073709551615,
  "b": true,
  "d": 1.7976931348623157e308,
  "s": "this is a string",
  "pulp level": "high",
  "y": {
    "d": -1.7976931348623157e308,
    "d2": Infinity
  },
  "v_y": [
    {
      "d": 0.0,
      "d2": 1.7976931348623157e308
    }
  ],
  "v_e": [
    "high",
    "medium",
    "low"
  ],
  "v_s": [
    "strings",
    "",
    "1"
  ]
})zzz";
  if (expected != actual)
  {
    std::cerr << __FILE__ << ":" << __LINE__ << ", FAIL, expected: " << expected
              << "\nactual: " << actual << "\n";
    return 1;
  }
  return 0;
}

int test_to_from_vector_objects()
{
  const std::string expected = R"zzz([
  {
    "d": 2.0,
    "d2": 1.0
  },
  {
    "d": 5.0,
    "d2": 6.0
  }
])zzz";
  int num_fails = 0;
  std::vector<Y> v_y;
  const auto status = easy_serialize::from_json_string_vector_objects(expected, v_y);
  const auto actual = easy_serialize::to_json_string_vector_objects(v_y);
  if (!status || expected != actual)
  {
    ++num_fails;
    std::cerr << __FILE__ << ":" << __LINE__ << ", FAIL, error: \"" << status.get_error_message()
              << "\"\nexpected: " << expected
              << "\nactual: " << actual << "\n";
  }
  const std::string filename = "test_vector_objects.json";
  const auto file_write_status = easy_serialize::to_json_file_vector_objects(filename, v_y);
  if (!file_write_status)
  {
    ++num_fails;
    std::cerr << __FILE__ << ":" << __LINE__ << ", FAIL, file write error: \""
              << file_write_status.get_error_message();
  }
  else
  {
    std::vector<Y> v_y2;
    const auto file_read_status = easy_serialize::from_json_file_vector_objects(filename, v_y2);
    if (!file_read_status)
    {
      ++num_fails;
      std::cerr << __FILE__ << ":" << __LINE__ << ", FAIL, file read error: \""
                << file_read_status.get_error_message();
    }
    else
    {
      auto v_y2_str = easy_serialize::to_json_string_vector_objects(v_y2);
      if (expected != v_y2_str)
      {
        ++num_fails;
        std::cerr << __FILE__ << ":" << __LINE__ << ", FAIL, "
                  << "\"\nexpected: " << expected
                  << "\nactual: " << v_y2_str << "\n";
      }
    }
  }
  std::remove(filename.c_str());
  return num_fails;
}

int test_to_from_vector()
{
  const std::string expected = R"zzz([
  "a",
  "vector",
  "of",
  "strings"
])zzz";
  int num_fails = 0;
  std::vector<std::string> v;
  const auto status = easy_serialize::from_json_string_vector(expected, v);
  const auto actual = easy_serialize::to_json_string_vector(v);
  if (!status || expected != actual)
  {
    ++num_fails;
    std::cerr << __FILE__ << ":" << __LINE__ << ", FAIL, error: \"" << status.get_error_message()
              << "\"\nexpected: " << expected
              << "\nactual: " << actual << "\n";
  }
  const std::string filename = "test_vector.json";
  const auto file_write_status = easy_serialize::to_json_file_vector(filename, v);
  if (!file_write_status)
  {
    ++num_fails;
    std::cerr << __FILE__ << ":" << __LINE__ << ", FAIL, file write error: \""
              << file_write_status.get_error_message();
  }
  else
  {
    std::vector<std::string> v2;
    const auto file_read_status = easy_serialize::from_json_file_vector(filename, v);
    if (!file_read_status)
    {
      ++num_fails;
      std::cerr << __FILE__ << ":" << __LINE__ << ", FAIL, file read error: \""
                << file_read_status.get_error_message();
    }
    else
    {
      auto v2_str = easy_serialize::to_json_string_vector(v);
      if (expected != v2_str)
      {
        ++num_fails;
        std::cerr << __FILE__ << ":" << __LINE__ << ", FAIL, "
                  << "\"\nexpected: " << expected
                  << "\nactual: " << v2_str << "\n";
      }
    }
  }
  std::remove(filename.c_str());
  return num_fails;
}

int test_to_from_object()
{
  const std::string expected = R"zzz({
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
    "d": 1.0,
    "d2": 2.0
  },
  "v_y": [
    {
      "d": 3.0,
      "d2": 4.0
    },
    {
      "d": 5.0,
      "d2": 6.0
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
  int num_fails = 0;
  Z z;
  const auto status = easy_serialize::from_json_string(expected, z);
  const auto actual = easy_serialize::to_json_string(z);
  if (!status || expected != actual)
  {
    ++num_fails;
    std::cerr << __FILE__ << ":" << __LINE__ << ", FAIL, error: \"" << status.get_error_message()
              << "\"\nexpected: " << expected
              << "\nactual: " << actual << "\n";
  }
  const std::string filename = "test_object.json";
  const auto file_write_status = easy_serialize::to_json_file(filename, z);
  if (!file_write_status)
  {
    ++num_fails;
    std::cerr << __FILE__ << ":" << __LINE__ << ", FAIL, file write error: \""
              << file_write_status.get_error_message();
  }
  else
  {
    Z z2;
    const auto file_read_status = easy_serialize::from_json_file(filename, z2);
    if (!file_read_status)
    {
      ++num_fails;
      std::cerr << __FILE__ << ":" << __LINE__ << ", FAIL, file read error: \""
                << file_read_status.get_error_message();
    }
    else
    {
      auto z2_str = easy_serialize::to_json_string(z2);
      if (expected != z2_str)
      {
        ++num_fails;
        std::cerr << __FILE__ << ":" << __LINE__ << ", FAIL, "
                  << "\"\nexpected: " << expected
                  << "\nactual: " << z2_str << "\n";
      }
    }
  }
  std::remove(filename.c_str());
  return num_fails;
}

int test_to_from_vector_enums()
{
  const std::string expected = R"zzz([
  "medium",
  "low",
  "high"
])zzz";
  int num_fails = 0;
  std::vector<OrangeJuicePulpLevel> v;
  const auto status = easy_serialize::from_json_string_vector_enums(expected, v, OrangeJuicePulpLevel::N);
  const auto actual = easy_serialize::to_json_string_vector_enums(v);
  if (!status || expected != actual)
  {
    ++num_fails;
    std::cerr << __FILE__ << ":" << __LINE__ << ", FAIL, error: \"" << status.get_error_message()
              << "\"\nexpected: " << expected
              << "\nactual: " << actual << "\n";
  }
  const std::string filename = "test_vector_enums.json";
  const auto file_write_status = easy_serialize::to_json_file_vector_enums(filename, v);
  if (!file_write_status)
  {
    ++num_fails;
    std::cerr << __FILE__ << ":" << __LINE__ << ", FAIL, file write error: \""
              << file_write_status.get_error_message();
  }
  else
  {
    std::vector<OrangeJuicePulpLevel> v2;
    const auto file_read_status = easy_serialize::from_json_file_vector_enums(filename, v, OrangeJuicePulpLevel::N);
    if (!file_read_status)
    {
      ++num_fails;
      std::cerr << __FILE__ << ":" << __LINE__ << ", FAIL, file read error: \""
                << file_read_status.get_error_message();
    }
    else
    {
      auto v2_str = easy_serialize::to_json_string_vector_enums(v);
      if (expected != v2_str)
      {
        ++num_fails;
        std::cerr << __FILE__ << ":" << __LINE__ << ", FAIL, "
                  << "\"\nexpected: " << expected
                  << "\nactual: " << v2_str << "\n";
      }
    }
  }
  std::remove(filename.c_str());
  return num_fails;
}

struct TestCase
{
  const char *const json;
  std::string expected_error;
};

template <class T>
int run_test_cases(int line, const char *function, const std::vector<TestCase> &test_cases)
{
  int num_fails = 0;
  for (const auto &tc : test_cases)
  {
    T tb;
    const auto status = easy_serialize::from_json_string(tc.json, tb);
    if (tc.expected_error != status.get_error_message())
    {
      ++num_fails;
      std::cerr << __FILE__ << ":" << line << ", FAIL, " << function << "(json: '" << tc.json
                << "'), expected: '" << tc.expected_error
                << "', actual: '" << status.get_error_message()
                << "'\n";
    }
  }
  return num_fails;
}

#define RUN_TEST_CASES(test_struct, test_cases) \
  run_test_cases<test_struct>(__LINE__, __FUNCTION__, test_cases)

struct TestBool
{
  bool t;
  template <class Archive>
  void serialize(Archive &ar)
  {
    ar.ez("k", t);
  }
};
int test_read_bool()
{
  std::vector<TestCase> test_cases = {
      {"{\"a\": true}", "[\"k\"] key not found"},
      {"{\"k\": 1}", "[\"k\"] expected a bool"},
      {"{\"k\": \"true\"}", "[\"k\"] expected a bool"},
  };
  return RUN_TEST_CASES(TestBool, test_cases);
}

struct TestI8
{
  int8_t t;
  template <class Archive>
  void serialize(Archive &ar)
  {
    ar.ez("k", t);
  }
};
int test_read_i8()
{
  std::vector<TestCase> test_cases = {
      {"{\"a\": true}", "[\"k\"] key not found"},
      {"{\"k\": -129}", "[\"k\"] expected an int8"},
      {"{\"k\": 128}", "[\"k\"] expected an int8"},
      {"{\"k\": \"grr\"}", "[\"k\"] expected an int8"},
  };
  return RUN_TEST_CASES(TestI8, test_cases);
}

struct TestI16
{
  int16_t t;
  template <class Archive>
  void serialize(Archive &ar)
  {
    ar.ez("k", t);
  }
};
int test_read_i16()
{
  std::vector<TestCase> test_cases = {
      {"{\"a\": true}", "[\"k\"] key not found"},
      {"{\"k\": -32769}", "[\"k\"] expected an int16"},
      {"{\"k\": 32768}", "[\"k\"] expected an int16"},
      {"{\"k\": 0.0}", "[\"k\"] expected an int16"},
  };
  return RUN_TEST_CASES(TestI16, test_cases);
}

struct TestI32
{
  int32_t t;
  template <class Archive>
  void serialize(Archive &ar)
  {
    ar.ez("k", t);
  }
};
int test_read_i32()
{
  std::vector<TestCase> test_cases = {
      {"{\"a\": true}", "[\"k\"] key not found"},
      {"{\"k\": -2147483649}", "[\"k\"] expected an int32"},
      {"{\"k\": 2147483648}", "[\"k\"] expected an int32"},
      {"{\"k\": false}", "[\"k\"] expected an int32"},
  };
  return RUN_TEST_CASES(TestI32, test_cases);
}

struct TestI64
{
  int64_t t;
  template <class Archive>
  void serialize(Archive &ar)
  {
    ar.ez("k", t);
  }
};
int test_read_i64()
{
  std::vector<TestCase> test_cases = {
      {"{\"a\": true}", "[\"k\"] key not found"},
      {"{\"k\": -9223372036854775809}", "[\"k\"] expected an int64"},
      {"{\"k\": 9223372036854775808}", "[\"k\"] expected an int64"},
      {"{\"k\": {}}", "[\"k\"] expected an int64"},
  };
  return RUN_TEST_CASES(TestI64, test_cases);
}

struct TestU8
{
  uint8_t t;
  template <class Archive>
  void serialize(Archive &ar)
  {
    ar.ez("k", t);
  }
};
int test_read_u8()
{
  std::vector<TestCase> test_cases = {
      {"{\"a\": true}", "[\"k\"] key not found"},
      {"{\"k\": -1}", "[\"k\"] expected a uint8"},
      {"{\"k\": 256}", "[\"k\"] expected a uint8"},
      {"{\"k\": \"grr\"}", "[\"k\"] expected a uint8"},
  };
  return RUN_TEST_CASES(TestU8, test_cases);
}

struct TestU16
{
  uint16_t t;
  template <class Archive>
  void serialize(Archive &ar)
  {
    ar.ez("k", t);
  }
};
int test_read_u16()
{
  std::vector<TestCase> test_cases = {
      {"{\"a\": true}", "[\"k\"] key not found"},
      {"{\"k\": -1}", "[\"k\"] expected a uint16"},
      {"{\"k\": 65536}", "[\"k\"] expected a uint16"},
      {"{\"k\": 0.0}", "[\"k\"] expected a uint16"},
  };
  return RUN_TEST_CASES(TestU16, test_cases);
}

struct TestU32
{
  uint32_t t;
  template <class Archive>
  void serialize(Archive &ar)
  {
    ar.ez("k", t);
  }
};
int test_read_u32()
{
  std::vector<TestCase> test_cases = {
      {"{\"a\": true}", "[\"k\"] key not found"},
      {"{\"k\": -1}", "[\"k\"] expected a uint32"},
      {"{\"k\": 4294967296}", "[\"k\"] expected a uint32"},
      {"{\"k\": false}", "[\"k\"] expected a uint32"},
  };
  return RUN_TEST_CASES(TestU32, test_cases);
}

struct TestU64
{
  uint64_t t;
  template <class Archive>
  void serialize(Archive &ar)
  {
    ar.ez("k", t);
  }
};
int test_read_u64()
{
  std::vector<TestCase> test_cases = {
      {"{\"a\": true}", "[\"k\"] key not found"},
      {"{\"k\": -1}", "[\"k\"] expected a uint64"},
      {"{\"k\": 18446744073709551616}", "[\"k\"] expected a uint64"},
      {"{\"k\": {}}", "[\"k\"] expected a uint64"},
  };
  return RUN_TEST_CASES(TestU64, test_cases);
}

struct TestDouble
{
  double t;
  template <class Archive>
  void serialize(Archive &ar)
  {
    ar.ez("k", t);
  }
};
int test_read_double()
{
  std::vector<TestCase> test_cases = {
      {"{\"a\": 0.0}", "[\"k\"] key not found"},
      {"{\"k\": {}}", "[\"k\"] expected a double"},
      {"{\"k\": 1}", "[\"k\"] expected a double"},
  };
  return RUN_TEST_CASES(TestDouble, test_cases);
}

struct TestString
{
  std::string t;
  template <class Archive>
  void serialize(Archive &ar)
  {
    ar.ez("k", t);
  }
};
int test_read_string()
{
  std::vector<TestCase> test_cases = {
      {"{\"a\": 0.0}", "[\"k\"] key not found"},
      {"{\"k\": false}", "[\"k\"] expected a string"},
  };
  return RUN_TEST_CASES(TestString, test_cases);
}

struct TestEnum
{
  OrangeJuicePulpLevel t;
  template <class Archive>
  void serialize(Archive &ar)
  {
    ar.ez_enum("k", t, OrangeJuicePulpLevel::N);
  }
};
int test_read_enum()
{
  std::vector<TestCase> test_cases = {
      {"{\"a\": 0.0}", "[\"k\"] key not found"},
      {"{\"k\": false}", "[\"k\"] expected a string"},
      {"{\"k\": \"dude\"}", "[\"k\"] expected an enum type"},
  };
  return RUN_TEST_CASES(TestEnum, test_cases);
}

struct TestObject
{
  Y t;
  template <class Archive>
  void serialize(Archive &ar)
  {
    ar.ez_object("k", t);
  }
};
int test_read_object()
{
  std::vector<TestCase> test_cases = {
      {"{\"a\": 0.0}", "[\"k\"] key not found"},
      {"{\"k\": false}", "[\"k\"] expected an object"},
      {"{\"k\": {}}", "[\"k\"][\"d\"] key not found"},
      {"{\"k\": {\"d\": true}}", "[\"k\"][\"d\"] expected a double"},
  };
  return RUN_TEST_CASES(TestObject, test_cases);
}

struct TestVector
{
  std::vector<int32_t> t;
  template <class Archive>
  void serialize(Archive &ar)
  {
    ar.ez_vector("k", t);
  }
};
int test_read_vector()
{
  std::vector<TestCase> test_cases = {
      {"{\"a\": 0.0}", "[\"k\"] key not found"},
      {"{\"k\": false}", "[\"k\"] expected an array"},
      {"{\"k\": {}}", "[\"k\"] expected an array"},
      {"{\"k\": [7, \"\"]}", "[\"k\"][1] expected an int32"},
  };
  return RUN_TEST_CASES(TestVector, test_cases);
}

struct TestVersionedObject
{
  bool b;
  int32_t i;
  template <class Archive>
  void serialize(Archive &ar)
  {
    ar.class_version(1);
    ar.ez("b", b);
    ar.ez("i", i, 1);
  }
};
int test_read_versioned_object()
{
  std::vector<TestCase> test_cases = {
      {"{\"b\": false}", ""},
      {"{\"_objver\": 0, \"b\": false}", ""},
      {"{\"_objver\": 1, \"b\": false}", "[\"i\"] key not found"},
      {"{\"_objver\": 1, \"b\": false, \"i\": 2}", ""},
      {"{\"_objver\": 2, \"b\": false, \"i\": 2}", "[\"_objver\"] object too new"},
  };
  return RUN_TEST_CASES(TestVersionedObject, test_cases);
}

int main()
{
  const int num_fails = test_writer_mins() + test_writer_maxes() +
                        test_to_from_object() + test_to_from_vector_objects() +
                        test_to_from_vector() + test_to_from_vector_enums() +
                        test_read_bool() + test_read_i8() + test_read_i16() +
                        test_read_i32() + test_read_i64() + test_read_u8() +
                        test_read_u16() + test_read_u32() + test_read_u64() +
                        test_read_double() + test_read_string() + test_read_enum() +
                        test_read_object() + test_read_vector() + test_read_versioned_object();

  return num_fails == 0 ? 0 : 1;
}
