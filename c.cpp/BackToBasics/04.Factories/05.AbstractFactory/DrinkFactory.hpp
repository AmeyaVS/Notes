#ifndef DRINKFACTORY_HPP
#define DRINKFACTORY_HPP

#include <string>
#include <map>
#include <memory>
#include <functional>
#include "HotDrinkFactory.hpp"

class DrinkFactory
{
public:
    DrinkFactory()
    {
        hot_factories["coffee"] = std::make_unique<CoffeeFactory>();
        hot_factories["tea"] = std::make_unique<TeaFactory>();
    }

    std::unique_ptr<HotDrink> make_drink(const std::string &name)
    {
        auto drink = hot_factories[name]->make();
        drink->prepare(200);
        return drink;
    }
private:
    std::map<std::string, std::unique_ptr<HotDrinkFactory>> hot_factories;
};

class DrinkWithVolumeFactory
{
    std::map<std::string, std::function<std::unique_ptr<HotDrink>()>> factories;
public:
    DrinkWithVolumeFactory()
    {
        factories["tea"] = [] {
            auto tea = std::make_unique<Tea>();
            tea->prepare(200);
            return tea;
        };

        factories["coffee"] = [] {
            auto tea = std::make_unique<Coffee>();
            tea->prepare(20);
            return tea;
        };
    }

    std::unique_ptr<HotDrink> make_drink(const std::string &name)
    {
        return factories[name]();
    }
};

#endif  //< DRINKFACTORY_HPP
