#include "svdpi.h"
//#include "Vour__Dpi.h"

#include <iostream>

// Dummy struct for exploration
struct A {
    A() { std::cout << "A()\n";}
    void foo() {
        std::cout << "A::Foo\n";
    }
    static void display() {
        std::cout << "static A::display\n";
    }
};

// Example use-case for calling with C++ Objects
A class_a;

// All DPI-C calls are following plain 'C' calling convention
// Hence needs to be wrapped up in extern "C" scope.
extern "C" {

int add(int a, int b) {
    A::display();
    class_a.foo();
    return a+b;
}

}
