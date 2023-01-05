HDRS := \
    include/easy_serialize/easy_serialize_status.hpp \
    include/easy_serialize/json_indent.hpp \
    include/easy_serialize/json_writer.hpp \
    include/easy_serialize/json_reader.hpp \
    include/easy_serialize/rapidjsonreader_impl.hpp \
    include/easy_serialize/rapidjsonwriter_impl.hpp \

WARNINGS := -Wpedantic -Wshadow -Wextra -Wconversion -Wunused -Wshadow -Werror -fsanitize=address,undefined

RAPIDJSON_FLAGS := -I../rapidjson/include -DRAPIDJSON_HAS_STDSTRING=1 -DRAPIDJSON_WRITE_DEFAULT_FLAGS=2

FLAGS := $(WARNINGS) $(RAPIDJSON_FLAGS) -Iinclude

main : main.cpp $(HDRS)
	g++ $(FLAGS) -O3 main.cpp -o $@

test_easy_serialize : test/test_easy_serialize.cpp $(HDRS)
	g++ $(FLAGS) -Og -g test/test_easy_serialize.cpp -o $@

.PHONY: test
test : test_easy_serialize
	./test_easy_serialize

.PHONY: clean
clean :
	@rm main
	@rm test_easy_serialize

# Note to build on Windows:
# cl.exe /EHsc /std:c++20 /Iinclude  /I..\rapidjson\include /D RAPIDJSAON_HAS_STDSTRING=1 /DRAPIDJSON_WRITE_DEFAULT_FLAGS=2 main.cpp /Femain.exe
# cl.exe /EHsc /std:c++20 /Iinclude  /I..\rapidjson\include /D RAPIDJSAON_HAS_STDSTRING=1 /DRAPIDJSON_WRITE_DEFAULT_FLAGS=2 test/test_easy_serialize.cpp /Fetest.exe