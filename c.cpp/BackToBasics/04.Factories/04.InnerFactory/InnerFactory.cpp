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
    float x, y;
public:
    friend std::ostream &operator<<(std::ostream &os, const Point &point)
    {
        os << "x: " << point.x << " y: " << point.y;
        return os;
    }

private:
    class PointFactory
    {
        PointFactory() {}
    public:
        static Point NewCartesian(float x, float y)
        {
            return {x, y};
        }

        static Point NewPolar(float r, float theta)
        {
            return {r * cos(theta), r * sin(theta)};
        }
    };
public:
    static PointFactory Factory;
};

int main()
{
    //auto p = Point::PointFactory::NewPolar(5, M_PI_4);  //< Valid only if Inner Class is publicly visible
    auto p = Point::Factory.NewPolar(5, M_PI_4);
    std::cout << p << std::endl;

    auto p2 = Point::Factory.NewCartesian(3, 4);
    std::cout << p2 << std::endl;

    //Point p2{3, 4};
    //std::cout << p2 << std::endl;

    return 0;
}
