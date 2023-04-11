#ifndef HOTDRINK_HPP
#define HOTDRINK_HPP

#include <iostream>
#include <memory>

struct HotDrink
{
    virtual ~HotDrink() = default;
    virtual void prepare(int volume) = 0;
};

struct Tea : public HotDrink
{
    void prepare(int volume) override
    {
        std::cout << "Take a tea bag, boil water, pour " << volume
                  << "ml, add some lemon\n";
    }
};

struct Coffee : public HotDrink
{
    void prepare(int volume) override
    {
        std::cout << "Grind some beans, boil water, pour " << volume
                  << "ml, add cream, enjoy!\n";
    }
};

#endif  //< HOTDRINK_HPP
