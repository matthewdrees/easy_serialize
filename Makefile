HDRS := \
    include/easy_serialize/easy_serialize_status.hpp \
    include/easy_serialize/json_indent.hpp \
    include/easy_serialize/json_writer.hpp \
    include/easy_serialize/json_reader.hpp \
    include/easy_serialize/rapidjsonreader_impl.hpp \
    include/easy_serialize/rapidjsonwriter_impl.hpp \

WARNINGS := -Wpedantic -Wshadow -Wextra -Wconversion -Wunused -Wshadow -Werror -fsanitize=address,undefined

RAPIDJSON_FLAGS := -DRAPIDJSON_HAS_STDSTRING=1 -DRAPIDJSON_WRITE_DEFAULT_FLAGS=2

main : main.cpp $(HDRS)
	g++ --std=c++17 $(WARNINGS) $(RAPIDJSON_FLAGS) -O3 -Iinclude -I../rapidjson/include main.cpp -o $@

test_easy_serialize : test/test_easy_serialize.cpp $(HDRS)
	g++ --std=c++17 $(WARNINGS) $(RAPIDJSON_FLAGS) -Og -g -Iinclude -I../rapidjson/include test/test_easy_serialize.cpp -o $@

.PHONY: test
test : test_easy_serialize
	./test_easy_serialize
