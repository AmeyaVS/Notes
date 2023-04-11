#include "gtest/gtest.h"

struct Square
{
    int side { 0 };
    explicit Square(const int side)
        : side(side)
    {}
};

struct Rectangle
{
    virtual int width() const = 0;
    virtual int height() const = 0;

    int area() const
    {
        return width() * height();
    }
};

struct SquareToRectangleAdapter : public Rectangle
{
    SquareToRectangleAdapter(const Square& square)
        : square(square)
    {

    }

    int width() const override
    {
        return square.side;
    }

    int height() const override
    {
        return square.side;
    }

    const Square& square;
};

namespace
{
    class Evaluate : public testing::Test
    {};

    TEST_F(Evaluate, SimpleTest)
    {
        Square sq { 11 };
        SquareToRectangleAdapter adapter { sq };
        ASSERT_EQ(121, adapter.area());
    }
}
