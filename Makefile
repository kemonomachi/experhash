erlang_base = $(shell erl -eval 'io:format("~s", [code:root_dir()])' -s init stop -noshell)/usr

CXXFLAGS += -std=c++11 -g $(shell pkg-config --cflags Magick++) -I$(erlang_base)/include -L$(erlang_base)/lib
LDFLAGS += -lei $(shell pkg-config --libs Magick++)

priv_dir = $(MIX_COMPILE_PATH)/../priv
out_dir = $(MIX_COMPILE_PATH)/../cpp/out

sources = $(wildcard cpp/src/*.cpp)
objects = $(patsubst cpp/src/%.cpp,$(out_dir)/%.o,$(sources))
header_deps = $(objects:.o=.d)

all: $(priv_dir)/experhash_port

$(priv_dir)/experhash_port: $(objects) | $(priv_dir)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(out_dir)/%.o: cpp/src/%.cpp | $(out_dir)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

$(priv_dir) $(out_dir):
	mkdir -p $@

$(out_dir)/%.d: cpp/src/%.cpp | $(out_dir)
	@set -e; \
	rm -f $@; \
	$(CXX) -MM -MT '$(@:.d=.o) $@' -MF $@ $(CPPFLAGS) $(CXXFLAGS) $<

include $(header_deps)

clean:
	rm -f $(priv_dir)/experhash_port $(objects) $(header_deps)

.PHONY: clean all

