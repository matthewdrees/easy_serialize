#pragma once

namespace easy_serialize
{

    enum class JsonIndent
    {
        compact,
        one_space,
        two_spaces,
        three_spaces,
        four_spaces,
    };

    inline unsigned get_num_spaces(JsonIndent json_indent)
    {
        switch (json_indent)
        {
        case JsonIndent::compact:
            break;
        case JsonIndent::one_space:
            return 1;
        case JsonIndent::two_spaces:
            return 2;
        case JsonIndent::three_spaces:
            return 3;
        case JsonIndent::four_spaces:
            return 4;
        }
        return 0;
    }

} // namespace easy_serialize
