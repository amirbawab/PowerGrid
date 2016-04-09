#pragma once

#include "City.h"
#include <memory>

class House
{
private:
    int price = 0;
    std::shared_ptr<City> city;

public:
    House();
    explicit House(std::shared_ptr<City> city);
    ~House();

    std::shared_ptr<City> GetCity() const { return city; }
    int GetPrice() const { return price; }
    void SetCity(std::shared_ptr<City> city) { this->city = city; }
    void SetPrice(int price) { this->price = price; }
};

