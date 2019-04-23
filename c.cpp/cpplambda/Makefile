PROJECT := cpplambda

CXX := g++

CXXFLAGS := -MMD -MP -std=c++14 -Wall -Wextra -Wpedantic -ggdb3

SRCS_CXX := cpplambda.cpp
OBJS := $(SRCS_CXX:%=%.o)
DEPS := $(OBJS:.o=.d)

all: $(PROJECT).run

$(PROJECT).run: $(OBJS)
	$(CXX) -o $@ $^

%.cpp.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	$(RM) $(PROJECT).run $(OBJS) $(DEPS)

-include $(DEPS)
