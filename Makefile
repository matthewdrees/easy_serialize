#libeasy_serialize.so : src/easy_serialize.cpp include/easy_serialize/easy_serialize.h
#	g++ -x c++ --std=c++11 -Wall -W -Wconversion -shared -fpic -DRAPIDJSON_HAS_STDSTRING=1 -O2 -Iinclude -I../rapidjson-master/include -Wl,-soname,libeasy_serialize.so -o $@ src/easy_serialize.cpp

HDRS := \
    include/easy_serialize/easy_serialize_status.hpp \
    include/easy_serialize/json_indent.hpp \
    include/easy_serialize/json_writer.hpp \
    include/easy_serialize/rapidjsonwriter_impl.hpp \

#WARNINGS := -Wpedantic -Wshadow -Wextra -Wconversion -Wunused -Wshadow -Werror -fsanitize=address,undefined

WARNINGS := -Wpedantic -Wshadow -Wextra -Wconversion -Wunused -Wshadow -Werror #-fsanitize=address,undefined

main : main.cpp $(HDRS)
	g++ --std=c++17 $(WARNINGS) -DRAPIDJSON_HAS_STDSTRING=1 -O3 -Iinclude -I../rapidjson-master/include main.cpp -o $@

test_easy_serialize : test/test_easy_serialize.cpp $(HDRS)
	g++ --std=c++17 $(WARNINGS) -DRAPIDJSON_HAS_STDSTRING=1 -Og -g -Iinclude -I../rapidjson-master/include test/test_easy_serialize.cpp -o $@

.PHONY: test
test : test_easy_serialize
	LD_LIBRARY_PATH=. ./test_easy_serialize
