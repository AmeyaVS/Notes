#include <iostream>
#include <cmath>

enum class PointType
{
    cartesian,
    polar
};

struct Point
{
    //Point(float x, float y) : x(x), y(y) {}

    //!
    //! \param a this is either x or rho
    //! \param b this is either y or theta
    //! \param type
    Point(float a, float b, PointType type = PointType::cartesian)
    {
        if(PointType::cartesian == type)
        {
            x = a;
            y = b;
        }
        else
        {
            x = a * cos(b);
            y = a * sin(b);
        }

    }

    float x, y;
};

int main()
{
    // Only example without demo since, this shows typical bad practice to create objects.
    // This function only exists from not breaking the build.
    return 0;
}
