PROJECT := coverage.run

CXX := g++

CXXFLAGS := -g -Wall -Wextra -Wpedantic -MMD -MP -c --coverage -O3 -fno-exceptions -fno-rtti

SRCS := coverage.cpp
OBJS := $(SRCS:%=%.o)
DEPS := $(OBJS:.o=.d)

all: $(PROJECT)
	./$(PROJECT)
	lcov -c -b . -d . -o coverage_test.info --rc lcov_branch_coverage=1 #--no-external
	lcov -a coverage_base.info -a coverage_test.info -o coverage_total.info --rc lcov_branch_coverage=1
	genhtml -o html coverage_total.info --branch-coverage --demangle-cpp --prefix /  #--prefix=/usr/include

$(PROJECT): $(OBJS)
	$(CXX) --coverage -o $@ $^
	lcov -c -i -b . -d . -o coverage_base.info #--rc lcov_branch_coverage=1 #--no-external

%.cpp.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	$(RM) -rf $(PROJECT) $(OBJS) $(DEPS) *.gcov *.gcda *.gcno *.info html

-include $(DEPS)
