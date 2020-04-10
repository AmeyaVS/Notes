#include <iostream>
//#define _USE_MATH_DEFINES
#include <cmath>

enum class PointType
{
    cartesian,
    polar
};

class Point
{
    Point(float x, float y) : x(x), y(y) {}
public:
    float x, y;

    static Point NewCartesian(float x, float y)
    {
        return {x, y};
    }

    static Point NewPolar(float r, float theta)
    {
        return { r * cos(theta), r * sin(theta)};
    }

    friend std::ostream &operator<<(std::ostream &os, const Point &point)
    {
        os << "x: " << point.x << " y: " << point.y;
        return os;
    }
};

int main()
{
    auto p = Point::NewPolar(5, M_PI_4);
    std::cout << p << std::endl;
    return 0;
}
