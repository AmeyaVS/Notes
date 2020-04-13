#include <iostream>
#include <string>

#include "HotDrink.hpp"
#include "DrinkFactory.hpp"


// This approach is poor man's implementation
std::unique_ptr<HotDrink> make_drink(const std::string &type)
{
    std::unique_ptr<HotDrink> drink;
    if(type == "tea")
    {
        drink = std::make_unique<Tea>();
        drink->prepare(200);
    }
    else
    {
        drink = std::make_unique<Coffee>();
        drink->prepare(50);
    }
    return drink;
}

int main()
{
    auto d = make_drink("tea");

    // Below Approach is much cleaner and extensible
    DrinkFactory df;

    auto c = df.make_drink("coffee");
    return 0;
}
