#include <iostream>
#include <string>
#include <memory>
#include "boost_local/di.hpp"

using namespace std;
using namespace boost;
using std::make_unique;
using std::make_shared;

struct Engine
{
    float volume = 5;
    int horse_power = 400;

    friend std::ostream& operator<<(std::ostream& os, const Engine& obj)
    {
        os << "volume: " << obj.volume
           << " horse_power: " << obj.horse_power;
        return os;
    }
};

struct ILogger
{
    virtual ~ILogger() = default;
    virtual void Log(const std::string& s) = 0;
};

struct ConsoleLogger : public ILogger
{
    ConsoleLogger() = default;

    void Log(const std::string& s) override
    {
        std::cout << "LOG: " << s.c_str() << std::endl;
    }
};

struct Car
{
    std::unique_ptr<Engine> engine;
    std::shared_ptr<ILogger> logger;

    Car(std::unique_ptr<Engine> engine, const std::shared_ptr<ILogger>& logger)
        : engine{move(engine)}
        , logger{logger}
    {
        logger->Log("making a car");
    }

    friend std::ostream& operator<<(std::ostream& os, const Car& obj)
    {
        os << "car with engine: " << *obj.engine;
        return os;
    }
};

int main()
{
    // manual
    auto logger = make_shared<ConsoleLogger>();
    auto c = make_shared<Car>(make_unique<Engine>(), logger);

    cout << *c << endl;

    auto injector = di::make_injector(
        di::bind<ILogger>().to<ConsoleLogger>()
    );

    auto car = injector.create<std::shared_ptr<Car>>();

    cout << *car << endl;
    return 0;
}