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
    //friend class PointFactory;  //< This breaks OCP
public:  //< This fixes the OCP but exposes private members/methods.
    Point(float x, float y) : x(x), y(y) {}
private:
    float x, y;
public:
    friend std::ostream &operator<<(std::ostream &os, const Point &point)
    {
        os << "x: " << point.x << " y: " << point.y;
        return os;
    }
};

struct PointFactory
{
    static Point NewCartesian(float x, float y)
    {
        return {x, y};
    }

    static Point NewPolar(float r, float theta)
    {
        return { r * cos(theta), r * sin(theta)};
    }
};

int main()
{
    auto p = PointFactory::NewPolar(5, M_PI_4);
    std::cout << p << std::endl;

    Point p2{3, 4};
    std::cout << p2 << std::endl;

    return 0;
}
