struct Point
{
    int x{ 0 }, y{ 0 };

    Point() = default;
    Point(const int x, const int y)
        : x{x}
        , y{y}
    {}
    //Point(const Point&) = default;
};

struct Line
{
    Point *start{ nullptr }, *end { nullptr };

    Line(Point* const start, Point* const end)
        : start(start)
        , end(end)
    {}

    ~Line()
    {
        if(nullptr != start) delete start;
        if(nullptr != end) delete end;
    }

    Line deep_copy() const
    {
        return Line{
            new Point{*start},//new Point(start->x, start->y),
            new Point{*end} //new Point(end->x, end->y)
        };
    }
};

#include "gtest/gtest.h"

namespace
{
    class Evaluate : public testing::Test
    {
    };

    TEST_F(Evaluate, SimpleTest)
    {
        Line line1{
            new Point{3, 3},
            new Point{10, 10}
        };

        auto line2 = line1.deep_copy();

        line1.start->x = line1.start->y = 0;
        line1.end->x = line1.end->y = 0;

        ASSERT_EQ(3, line2.start->x);
        ASSERT_EQ(3, line2.start->y);
        ASSERT_EQ(10, line2.end->x);
        ASSERT_EQ(10, line2.end->y);
    }

    TEST_F(Evaluate, AnotherTest)
    {
        Line line1{
            new Point{3, 4},
            new Point{10, 11}
        };

        auto line2 = line1.deep_copy();

        line1.start->x = line1.start->y = 0;
        line1.end->x = line1.end->y = 0;

        ASSERT_EQ(3, line2.start->x);
        ASSERT_EQ(4, line2.start->y);
        ASSERT_EQ(10, line2.end->x);
        ASSERT_EQ(11, line2.end->y);
    }
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}