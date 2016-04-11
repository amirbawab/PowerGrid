#pragma once

#include <HouseColor.h>
#include <memory>

class City;

class House
{
private:
    int price = 0;
    std::shared_ptr<City> city;
    std::shared_ptr<HouseColor> color;

public:
    House(std::shared_ptr<City> city, std::shared_ptr<HouseColor> color);
    ~House();

    std::shared_ptr<City> GetCity() const { return city; }

    int GetPrice() const { return price; }
    void SetCity(std::shared_ptr<City> city) { this->city = city; }
    std::shared_ptr<HouseColor> GetColor() const { return color; }

    void SetPrice(int price) { this->price = price; }
};

