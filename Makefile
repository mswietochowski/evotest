CXX		?= g++
LDFLAGS += -lgsl -lgslcblas

all: dist/evotest

dist/evotest: objs/graph.o objs/main.o
	@exec mkdir -p -- dist
	exec $(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

objs/%.o: src/%.cpp
	@exec mkdir -p -- $(dir $@)
	exec $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean::
	exec rm -rf -- objs

distclean:: clean
	exec rm -rf -- dist program


