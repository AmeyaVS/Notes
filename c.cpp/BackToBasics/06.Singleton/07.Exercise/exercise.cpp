#include <functional>

using namespace std;

struct SingletonTester
{
    template<typename T>
    bool is_singleton(function<T*()> factory)
    {
        T* _1 = factory();
        T* _2 = factory();
        return (_1 == _2);
    }
};

#include "gtest/gtest.h"

namespace
{
    class Evaluate : public testing::Test
    {};

    TEST_F(Evaluate, SimpleTest)
    {
        SingletonTester tester;
        ASSERT_TRUE(tester.is_singleton<SingletonTester>(
            [&tester]() -> SingletonTester* { return &tester; }))
            << "Expected a singleton in this case; provided lambda is yielding the same object over and over.";
        ASSERT_FALSE(tester.is_singleton<SingletonTester>([]() { return new SingletonTester{};}));
    }
}  //< namespace