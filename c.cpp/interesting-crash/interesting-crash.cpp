// Foo.h
struct Foo {
  Foo();
};

Foo f;

#include <iostream>

Foo::Foo() {
  std::cerr << "In destructor!" << std::endl;
}

int main() {
  return 0;
}
