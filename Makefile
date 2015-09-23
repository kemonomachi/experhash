erlang_base = $(shell erl -eval 'io:format("~s", [code:root_dir()])' -s init stop -noshell)/usr

CXXFLAGS += -std=c++11 -g $(shell pkg-config --cflags Magick++) -I$(erlang_base)/include -L$(erlang_base)/lib
LDFLAGS += -lei $(shell pkg-config --libs Magick++)

sources = $(wildcard cpp/src/*.cpp)
objects = $(sources:.cpp=.o)
header_deps = $(sources:.cpp=.d)

priv/experhash_port: $(objects) | priv
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

priv:
	mkdir -p $@

cpp/src/%.d: cpp/src/%.cpp
	@set -e; \
	rm -f $@; \
	$(CXX) -MM -MT '$(@:.d=.o) $@' -MF $@ $(CPPFLAGS) $(CXXFLAGS) $<

include $(header_deps)

.PHONY: clean
clean:
	rm -f priv/exphash_port $(objects) $(header_deps)

