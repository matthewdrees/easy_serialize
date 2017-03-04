#pragma once
// Parse a json string into a compound map/series object.
//
//
#include <map>

namespace easy_serialize {

enum class JsonType {
    Object,
    Array,
    Field
};
   
class JsonValue
{
public:
    /**
     * Parse an istream and return a populated JsonValue.
     */
    JsonValue(std::istream& istream);

    explicit JsonValue(JsonType t) : jsonType(t), object(), array(), field() {}

    JsonType jsonType;

    // This value is one of these types, depending on jsonType.
    // Think of this like a union, but without the memory
    // savings. Not even boost::variant can save us here.
    std::map<std::string, JsonValue> object;
    std::vector<JsonValue> array;
    std::string field;
};

} // namespace easy_serialize

