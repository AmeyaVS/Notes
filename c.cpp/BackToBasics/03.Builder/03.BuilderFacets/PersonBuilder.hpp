#ifndef PERSONBUILDER_H
#define PERSONBUILDER_H

#include "Person.hpp"

class PersonAddressBuilder;
class PersonJobBuilder;

class PersonBuilderBase
{
protected:
    Person &person;
    explicit PersonBuilderBase(Person &person)
        : person{person}
    {
    }

public:
    PersonAddressBuilder lives() const;
    PersonJobBuilder works() const;

    operator Person()
    {
        return std::move(person);
    }
}; //< PersonBuilderBase

class PersonBuilder : public PersonBuilderBase
{
    Person p;

public:
    explicit PersonBuilder() : PersonBuilderBase{p}
    {
    }

}; //< PersonBuilder

#endif //< PERSONBUILDER_H