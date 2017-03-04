libeasy_serialize.so : src/easy_serialize.cpp include/easy_serialize/easy_serialize.h
	g++ -x c++ --std=c++14 -Wall -W -Wconversion -shared -fpic -Iinclude -I../double-conversion-master -l../double-conversion-master/libdouble-conversion.a -Wl,-soname,libeasy_serialize.so -o $@ src/easy_serialize.cpp

test_easy_serialize : test/test_easy_serialize.cpp libeasy_serialize.so
	g++ -x c++ --std=c++14 -Wall -W -Wconversion -Iinclude -I../double-conversion-master test/test_easy_serialize.cpp -L. -leasy_serialize -o $@

test_parse_json : test/test_parse_json.cpp libeasy_serialize.so
	g++ -x c++ --std=c++14 -Wall -W -Wconversion -Iinclude -I../double-conversion-master test/test_parse_json.cpp -L. -leasy_serialize -o $@

.PHONY: test
test : test_easy_serialize
	LD_LIBRARY_PATH=. ./test_easy_serialize

.PHONY: test
test : test_parse_json
	LD_LIBRARY_PATH=. ./test_parse_json
