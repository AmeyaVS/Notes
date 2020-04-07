#include "Person.hpp"
#include "PersonBuilder.hpp"

PersonBuilder Person::create()
{
    return PersonBuilder();
}

std::ostream &operator<<(std::ostream &os, const Person &person)
{
    os << "street_address: " << person.street_address
       << " post_code: " << person.post_code
       << " city: " << person.city
       << " company_name: " << person.company_name
       << " position: " << person.company_name
       << " annual_income: " << person.annual_income;
    return os;
}
