PROJECT := coverage.run

CXX := g++
RM := rm -rf

CXXFLAGS := -g -Wall -Wextra -Wpedantic -MMD -MP -c --coverage -O0 -fno-exceptions -fno-rtti

BUILD_DIR := build
SRCS := coverage.cpp
OBJS := $(BUILD_DIR)/$(SRCS:%=%.o)
DEPS := $(OBJS:.o=.d)


all: $(BUILD_DIR)/$(PROJECT)
	./$(BUILD_DIR)/$(PROJECT)
	lcov -c -b . -d $(BUILD_DIR) -o $(BUILD_DIR)/coverage_test.info --rc lcov_branch_coverage=1 --no-external
	lcov -a $(BUILD_DIR)/coverage_base.info -a $(BUILD_DIR)/coverage_test.info -o $(BUILD_DIR)/coverage_total.info --rc lcov_branch_coverage=1 #--no-external
	genhtml -o $(BUILD_DIR)/CoverageHtml $(BUILD_DIR)/coverage_total.info --branch-coverage --demangle-cpp #--prefix #/  #--prefix=/usr/include

$(BUILD_DIR)/$(PROJECT): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) --coverage -o $@ $^
	lcov -c -i -b . -d $(BUILD_DIR) -o $(BUILD_DIR)/coverage_base.info --no-external #--rc lcov_branch_coverage=1 #--no-external

$(BUILD_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	$(RM) $(BUILD_DIR)/$(PROJECT) $(OBJS) $(DEPS) $(BUILD_DIR)/*.gcno $(BUILD_DIR)/*.gcda $(BUILD_DIR)/*.info $(BUILD_DIR)/CoverageHtml

-include $(DEPS)
