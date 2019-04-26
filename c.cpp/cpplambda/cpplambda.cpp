#include <typeinfo>
#include <cstdio>
#include <iostream>

#define GLOBAL_DECL 1

#if GLOBAL_DECL
struct __main_lambda_0
{
  auto operator()() const {
    std::cout << "function: " <<  __PRETTY_FUNCTION__ << std::endl;
    return 5;
  }
};

struct __main_lambda_1
{
  auto operator()(int i) const {
    std::cout << "function: " <<  __PRETTY_FUNCTION__ << std::endl;
    return 5 + i;
  }
};

struct __main_lambda_2
{
  int val;
  auto operator()(int i) const {
    std::cout << "function: " <<  __PRETTY_FUNCTION__ << std::endl;
    return 5 + i + val;
  }
};

struct __main_lambda_3
{
  int val;
  template<typename T>
  auto operator()(T i) {
    std::cout << "function: " <<  __PRETTY_FUNCTION__ << std::endl;
    return 5 + i + val;
  }
};
#endif  // GLOBAL_DECL

int main() {
#if !GLOBAL_DECL
  struct __main_lambda_0
  {
    auto operator()() const {
      std::cout << "function: " <<  __PRETTY_FUNCTION__ << std::endl;
      return 5;
    }
  };

  struct __main_lambda_1
  {
    auto operator()(int i) const {
      std::cout << "function: " <<  __PRETTY_FUNCTION__ << std::endl;
      return 5 + i;
    }
  };

  struct __main_lambda_2
  {
    int val;
    auto operator()(int i) const {
      std::cout << "function: " <<  __PRETTY_FUNCTION__ << std::endl;
      return 5 + i + val;
    }
  };

  struct __main_lambda_3
  {
    int val;
    template<typename T>
      auto operator()(T i) {
        std::cout << "function: " <<  __PRETTY_FUNCTION__ << std::endl;
        return 5 + i + val;
      }
  };
#endif  // !GLOBAL_DECL

  auto l = []() {
    std::cout << "function: " <<  __PRETTY_FUNCTION__ << std::endl;
    return 5;
  };
  puts(typeid(l).name());
  l();

  __main_lambda_0 ml0;
  ml0();

  auto l1 = [](int i) {
    std::cout << "function: " <<  __PRETTY_FUNCTION__ << std::endl;
    return 5 + i;
  };

  puts(typeid(l1).name());
  l1(3);

  __main_lambda_1 ml1;
  ml1(3);

  int val = 10;
  auto l2 = [val](int i) {
    std::cout << "function: " <<  __PRETTY_FUNCTION__ << std::endl;
    return 5 + i + val;
  };
  puts(typeid(l2).name());
  l2(3);

  auto l2_1 = __main_lambda_2{val};

  l2_1(3);

  auto l3 = [val](int i) mutable {
    std::cout << "function: " <<  __PRETTY_FUNCTION__ << std::endl;
    return 5 + i + ++val;
  };
  puts(typeid(l3).name());
  l3(10);

  __main_lambda_3 ml3{3};
  ml3(6);
  return 0;
}
