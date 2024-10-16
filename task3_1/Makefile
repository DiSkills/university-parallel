SRCMODULES = array.cpp
OBJMODULES = $(SRCMODULES:.cpp=.o)

CXX = g++
CXXFLAGS = -ggdb -Wall
CXXFLAGS += -DDEBUG

%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

prog: main.cpp $(OBJMODULES)
	$(CXX) $(CXXFLAGS) $^ -o $@

deps.mk: $(SRCMODULES)
	$(CXX) -MM $^ > $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

clean:
	rm -f *.o prog deps.mk
