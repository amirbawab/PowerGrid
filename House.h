#pragma once

#include "City.h"
#include <memory>

class House
{
private:
    int price;
    std::shared_ptr<City> city;

public:
    House();
    House(std::shared_ptr<City> city);
    ~House();

    std::shared_ptr<City> GetCity() const { return city; }
    int GetPrice() const { return price; }
};

