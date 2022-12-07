#pragma once

namespace easy_serialize
{

    // Populate object T
    template <typename T>
    EasySerializeStatus from_json_file(const std::string &filename, T &obj)
    {
    }

    // Populate object T
    template <typename T>
    EasySerializeStatus from_json_file(const std::string &filename, std::vector<T> &v)
    {
    }

} // namespace easy_serialize
