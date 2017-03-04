
#include "json_parser.h"

namespace easy_serialize {

JsonValue::JsonValue(std::istream& in)
{
    char c;
    while (in.get(c)) {
        switch (c) {
        case ' ':
        case '\n':
        case '\r':
        case '\t':
            // Skip whitespace.
            break;

        case '[':
            this = getJsonArrayFromStream(in);
            break;

        case '{':
            this = getJsonObjectFomStream(in);
            break;
        
        default:
            throw runtime_error("expected { or [.");
        }
    }
}

enum class ParseState {
    NeedKeyOrEnd,
    NeedColon,
    NeedValue,
    NeedValueOrEnd,
    Done,
};

std::string getRemainingString(std::istream& in)
{
    std::string s;
    char c;
    bool escaped = false;
    int utf8charbytesleft = 0;
    while (in.get(c)) {
        if (c >= 32 && c < 127) { // Valid ascii.
            if (utf8charbytesleft > 0) {
                throw std::RuntimeException("Badly-formed UTF-8.");
            }
            if (c == '"' && !escaped) {
                break;
            }
            if (c == '\\') {
                escaped = !escaped;
            }
            else {
                escaped = false;
            }
        }
        else if (c < 0) {
            if (utf8charbytesleft == 0) {
                if (!(c & 0b01000000)) {
                    utf8charbytesleft = 1;
                }
                else if (!(c & 0b00100000)) {
                    utf8charbytesleft = 2;
                }
                else if (!(c & 0b00010000)) {
                    utf8charbytesleft = 3;
                }
                else if (!(c & 0b00001000)) {
                    utf8charbytesleft = 4;
                }
                else {
                    throw std::RuntimeException("Badly-formed UTF-8");
                }
            }
            // In the middle of a utf8 character.
            else {
                if (!(c & 0b10000000)) {
                    throw std::RuntimeException("Badly-formed UTF-8");
                }
                --utf8charbytesleft;
            }
        }
        s += c;
    }
    return s;
}

JsonValue getJsonObjectFromStream(std::istream& in)
{
    // Already have taken leading '{' character.
    auto obj = JsonValue(JsonType::Object);

    auto parse_state = ParseState::NeedKeyOrEnd;
    std::string key;
    std::string value;
    char c;
    while (in.get(c)) {
        switch (c) {
        case ' ':
        case '\n':
        case '\r':
        case '\t':
            // Skip whitespace.
            break;

        case '"':
            switch (parse_state) {
            case ParseState::NeedKey:
                key = getRemainingString(in);
                parse_state = ParseState::NeedColon;
                break;
            case ParseState::NeedValue:
                {
                    auto value = JsonValue(JsonType::Field);
                    value.field = getRemainingString(in);
                    obj.value[key] = std::move(value);
                }
                parse_state = ParseState::NeedCommaOrEnd;
                break;
            default:
                throw std::RuntimeException("Badly-formed UTF-8");
            }
            break;

        case ':':
            if (parse_state != ParseState::NeedColon) {
                throw std::RuntimeException("Badly-formed UTF-8");
            }
            break;

        case ',':
            if (parse_state != ParseState::NeedCommaOrEnd) {
                throw std::RuntimeException("Badly-formed UTF-8");
            }
            break;

        case '[':
            if (parse_state == ParseState::NeedValue) {
                auto value = JsonValue(JsonType::Array);
                value.array = getRemainingArray(in);
                obj.value[key] = std::move(value);
                parse_state = ParseState::NeedCommaOrEnd;
            }
            else {
                throw std::RuntimeException("Badly-formed UTF-8");
            }
        case '}':
            if (parse_state == ParseState::NeedCommaOrEnd) {
                parse_state = ParseState::Done;
            else {
                throw std::RuntimeException("Found unexpected '}'.");
            }
            break;

        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            // All of these for a json number. Fall thru.
        case 't':
        case 'f':
            // These for json true or false. Fall thru.

            if (parse_state == ParseState::NeedValue) {
                {
                    auto strVal = getRemainingField(in);
                    auto val = JsonValue(JsonType::Field);
                    val.value[strVal] = JsonValue(JsonType::Field);
                    obj.value[key] = std::move(val);
                }
            }
            break;

        default:
            throw std::RuntimeException("Badly-formed UTF-8");
            break;
        }

        if (parse_state == ParseState::Done) {
            break;
        }
    }
   
    if (parse_state != ParseState::Done) {
        throw std::RuntimeException("Badly-formed UTF-8");
    }
    return obj;
}

JsonValue getJsonArrayFromStream(std::istream& istream)
{
    auto arr = JsonValue(JsonType::Array);
    // TODO: this
    return arr;
}

// namespace easy_serialize

