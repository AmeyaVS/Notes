#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <string>

class PersonBuilder;

class Person
{
    // address
    std::string street_address, post_code, city;

    // employment
    std::string company_name, position;
    int annual_income{0};

    Person() = default;
    Person(const Person&) = delete;
    Person& operator=(const Person &other) = delete;

public:
    ~Person() = default;

    static PersonBuilder create();

    Person(Person &&other)
        : street_address{std::move(other.street_address)},
          post_code{std::move(other.post_code)},
          city{std::move(other.city)},
          company_name{std::move(other.company_name)},
          position{std::move(other.position)},
          annual_income{other.annual_income}
    {
    }

    Person &operator=(Person &&other)
    {
        if (this == &other)
        {
            return *this;
        }
        street_address = std::move(other.street_address);
        post_code = std::move(other.post_code);
        city = std::move(other.city);
        company_name = std::move(other.company_name);
        position = std::move(other.position);
        annual_income = other.annual_income;
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const Person &person);

private:
    friend class PersonBuilder;
    friend class PersonJobBuilder;
    friend class PersonAddressBuilder;
}; //< Person

#endif //< PERSON_H