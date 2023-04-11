#ifndef HOTDRINKFACTORY_HPP
#define HOTDRINKFACTORY_HPP

#include <memory>
#include "HotDrink.hpp"

struct HotDrinkFactory
{
    virtual std::unique_ptr<HotDrink> make() const = 0;
};

struct TeaFactory : public HotDrinkFactory
{
    std::unique_ptr<HotDrink> make() const override
    {
        return std::make_unique<Tea>();
    }
};

struct CoffeeFactory : public HotDrinkFactory
{
    std::unique_ptr<HotDrink> make() const override
    {
        return std::make_unique<Coffee>();
    }
};

#endif  //< HOTDRINKFACTORY_HPP
