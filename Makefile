libeasy_serialize.so : src/easy_serialize.cpp include/easy_serialize/easy_serialize.h
	g++ -x c++ --std=c++11 -Wall -W -Wconversion -shared -fpic -DRAPIDJSON_HAS_STDSTRING=1 -O2 -Iinclude -I../rapidjson-master/include -Wl,-soname,libeasy_serialize.so -o $@ src/easy_serialize.cpp

test_easy_serialize : test/test_easy_serialize.cpp libeasy_serialize.so
	g++ -x c++ --std=c++11 -Wall -W -Wconversion -DRAPIDJSON_HAS_STDSTRING=1 -O2 -Iinclude -I../rapidjson-master/include test/test_easy_serialize.cpp -L. -leasy_serialize -o $@

.PHONY: test
test : test_easy_serialize
	LD_LIBRARY_PATH=. ./test_easy_serialize
